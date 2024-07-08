#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/video_stream_header.h"
#include "mediapipe/framework/port/logging.h"
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

            cv::BFMatcher bf_matcher_;
            std::vector<int> match_counts(qfeatures.size(), 0);
            // Process each query image
            int selectionIndex = 0;
            for (const auto &encoded_descriptor : qfeatures)
            {
                // Deserialize query features for this image
                cv::Mat query_descriptors = deserialize_orb_descriptors(encoded_descriptor);

                // Perform matching
                std::vector<std::vector<cv::DMatch>> matches;
                bf_matcher_.knnMatch(query_descriptors, features, matches, 2); // k=2 for ratio test

                // Apply ratio test and count matches
                int localMatchCount = 0;
                for (const auto &match : matches)
                {
                    if (match.size() == 2 && match[0].distance < 0.75 * match[1].distance)
                    {
                        localMatchCount++;
                    }
                }
                match_counts[selectionIndex] = localMatchCount;
            }

            // Find the index with the maximum match count
            int max_index = -1;
            int max_count = 20;
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
        std::vector<uint8_t> base64_decode(const std::string &encoded_string)
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

        cv::Mat deserialize_orb_descriptors(const std::string &encoded_descriptors)
        {
            // Decode base64 string
            std::vector<uint8_t> descriptor_data = base64_decode(encoded_descriptors);

            // Determine the number of descriptors and descriptor size
            // ORB descriptors are typically 32 bytes each
            int descriptor_size = 32;
            int num_descriptors = descriptor_data.size() / descriptor_size;

            // Create cv::Mat and copy data
            cv::Mat descriptors(num_descriptors, descriptor_size, CV_8U);
            std::memcpy(descriptors.data, descriptor_data.data(), descriptor_data.size());

            return descriptors;
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