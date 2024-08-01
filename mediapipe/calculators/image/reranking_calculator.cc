#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/formats/video_stream_header.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_features2d_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/threadpool.h"
#include "mediapipe/framework/tool/options_util.h"
#include "absl/memory/memory.h"
#include "absl/synchronization/blocking_counter.h"

namespace mediapipe
{

    class RerankCalculator : public CalculatorBase
    {
    public:
        static absl::Status GetContract(CalculatorContract *cc)
        {
            // cc->SetProcessTimestampBounds(true);
            if (cc->Inputs().HasTag("FEATURES"))
            {
                cc->Inputs().Tag("FEATURES").Set<cv::Mat>();
            }
            if (cc->Inputs().HasTag("QFEATURES"))
            {
                cc->Inputs().Tag("QFEATURES").Set<std::string>();
            }
            if (cc->Outputs().HasTag("RESULT"))
            {
                cc->Outputs().Tag("RESULT").Set<int>();
            }
            return absl::OkStatus();
        }

        absl::Status Open(CalculatorContext *cc) override
        {
            cc->SetOffset(TimestampDiff(0));
            matcher_ = cv::makePtr<cv::FlannBasedMatcher>(cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2));
            return absl::OkStatus();
        }

        absl::Status Process(CalculatorContext *cc) override
        {
            if (cc->Inputs().Tag("FEATURES").IsEmpty() || cc->Inputs().Tag("QFEATURES").IsEmpty())
            {
                auto output = absl::make_unique<int>(-1);
                cc->Outputs().Tag("RESULT").Add(output.release(), cc->InputTimestamp());
                return absl::OkStatus();
            }

            const cv::Mat &features = cc->Inputs().Tag("FEATURES").Get<cv::Mat>();
            const std::string &qfeaturesstring = cc->Inputs().Tag("QFEATURES").Get<std::string>();
            const std::vector<std::string> &qfeatures = splitQueryFeatures(qfeaturesstring);
            std::vector<int> match_counts(qfeatures.size(), -1);

            // Process each query image
            for (int selectionIndex = 0; selectionIndex < qfeatures.size(); ++selectionIndex)
            {
                const auto &encoded_descriptor = qfeatures[selectionIndex];

                // Deserialize query features for this image
                cv::Mat query_descriptors;
                try
                {
                    query_descriptors = deserialize_orb_descriptors(encoded_descriptor);
                }
                catch (const std::runtime_error &e)
                {
                    LOG(ERROR) << "Failed to deserialize ORB descriptors: " << e.what();
                    return absl::InternalError("Failed to deserialize ORB descriptors.");
                }

                // Perform matching
                std::vector<std::vector<cv::DMatch>> matches;
                matcher_->knnMatch(query_descriptors, features, matches, 2); // k=2 for ratio test

                // Apply ratio test and count matches
                int localMatchCount = 0;
                for (const auto &match : matches)
                {
                    if (match.size() == 2 && match[0].distance < 0.7f * match[1].distance)
                    {
                        localMatchCount++;
                    }
                }
                match_counts[selectionIndex] = localMatchCount;
            }

            // Find the index with the maximum match count
            int max_index = -1;
            int max_count = 10;
            for (int i = 0; i < match_counts.size(); ++i)
            {
                if (match_counts[i] > max_count)
                {
                    max_index = i;
                    max_count = match_counts[i];
                }
            }

            // Output the index with the maximum match count
            auto output = absl::make_unique<int>(max_index);
            cc->Outputs().Tag("RESULT").Add(output.release(), cc->InputTimestamp());

            return absl::OkStatus();
        }

    private:
        cv::Ptr<cv::FlannBasedMatcher> matcher_;
        std::vector<uint8_t> base64_decode_custom(const std::string &encoded_string)
        {
            const std::string base64_chars =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz"
                "0123456789+/";

            std::vector<uint8_t> ret;
            int i = 0;
            int j = 0;
            uint8_t char_array_4[4], char_array_3[3];

            for (char c : encoded_string)
            {
                if (c == '=' || base64_chars.find(c) == std::string::npos)
                    continue;

                char_array_4[i++] = c;
                if (i == 4)
                {
                    for (i = 0; i < 4; i++)
                        char_array_4[i] = base64_chars.find(char_array_4[i]);

                    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                    for (i = 0; i < 3; i++)
                        ret.push_back(char_array_3[i]);
                    i = 0;
                }
            }

            if (i)
            {
                for (j = i; j < 4; j++)
                    char_array_4[j] = 0;

                for (j = 0; j < 4; j++)
                    char_array_4[j] = base64_chars.find(char_array_4[j]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (j = 0; j < i - 1; j++)
                    ret.push_back(char_array_3[j]);
            }

            return ret;
        }

        cv::Mat deserialize_orb_descriptors(const std::string &base64_str)
        {
            // Decode from base64
            // std::string decoded_string = base64_decode(base64_str);
            // uint32_t rows, cols, type;
            // std::memcpy(&rows, decoded.data(), sizeof(uint32_t));
            // std::memcpy(&cols, decoded.data() + sizeof(uint32_t), sizeof(uint32_t));
            // std::memcpy(&type, decoded.data() + 2 * sizeof(uint32_t), sizeof(uint32_t));

            // // Create cv::Mat and copy data
            // cv::Mat descriptors(rows, cols, CV_8U);
            // std::memcpy(descriptors.data, decoded.data() + 3 * sizeof(uint32_t), rows * cols * type);

            // return descriptors;
            std::vector<uint8_t> decoded = base64_decode_custom(base64_str);

            // Extract header information (rows, cols, type)
            const uint8_t *data = decoded.data();
            uint32_t rows = *reinterpret_cast<const uint32_t *>(data);
            uint32_t cols = *reinterpret_cast<const uint32_t *>(data + sizeof(uint32_t));
            uint32_t type_size = *reinterpret_cast<const uint32_t *>(data + 2 * sizeof(uint32_t));

            // Determine OpenCV type (handle more types)
            int type_cv;
            if (type_size == sizeof(uint8_t))
            {
                type_cv = CV_8U;
            }
            else if (type_size == sizeof(float))
            {
                type_cv = CV_32F;
            }
            else if (type_size == sizeof(int))
            {
                type_cv = CV_32S; // Example: Handle int
            }
            else if (type_size == sizeof(double))
            {
                type_cv = CV_64F; // Example: Handle double
            }
            else
            {
                throw std::runtime_error("Unsupported data type size.");
            }

            // Extract matrix data (handle data types correctly)
            const uint8_t *matrix_data = data + 3 * sizeof(uint32_t);

            // Allocate memory for the descriptor data
            cv::Mat descriptors(rows, cols, type_cv);

            // Copy the data to the allocated memory (use memcpy for the specific data type)
            if (type_size == sizeof(uint8_t))
            {
                std::memcpy(descriptors.data, matrix_data, rows * cols * sizeof(uint8_t));
            }
            else if (type_size == sizeof(float))
            {
                std::memcpy(descriptors.data, matrix_data, rows * cols * sizeof(float));
            }
            else if (type_size == sizeof(int))
            {
                std::memcpy(descriptors.data, matrix_data, rows * cols * sizeof(int));
            }
            else if (type_size == sizeof(double))
            {
                std::memcpy(descriptors.data, matrix_data, rows * cols * sizeof(double));
            }
            else
            {
                throw std::runtime_error("Unsupported data type size.");
            }
            return descriptors.clone();
        }

        std::vector<std::string> splitQueryFeatures(const std::string &input, char delimiter = '|')
        {
            std::vector<std::string> result;
            std::istringstream stream(input);
            std::string token;

            while (std::getline(stream, token, delimiter))
            {
                if (!token.empty())
                {
                    result.push_back(token);
                }
            }

            return result;
        }
    };
    REGISTER_CALCULATOR(RerankCalculator);
} // namespace mediapipe