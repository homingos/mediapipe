#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/port/opencv_imgcodecs_inc.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/calculators/image/feature_detector_calculator_custom.h"
#include "mediapipe/framework/port/parse_text_proto.h"

namespace mediapipe {
// Helper function to read file contents
std::string ReadFileContents(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return "";
  }

  std::string contents((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
  file.close();

  if (contents.empty()) {
    std::cerr << "File is empty or failed to read: " << filename << std::endl;
  }

  return contents;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " GRAPH_FILE IMAGE_FILE" << std::endl;
    return 1;
  }

  // Read the graph configuration from the .pbtxt file
  std::string graph_config_contents = ReadFileContents(argv[1]);
  if (graph_config_contents.empty()) {
    std::cerr << "Failed to read graph config from: " << argv[1] << std::endl;
    return 1;
  }

  CalculatorGraphConfig config;
  if (!ParseTextProto(graph_config_contents, &config)) {
    std::cerr << "Failed to parse graph config." << std::endl;
    return 1;
  }

  std::cout << "Successfully read and parsed graph config." << std::endl;

  // Read the input image
  cv::Mat image = cv::imread(argv[2]);
  if (image.empty()) {
    std::cerr << "Failed to read image: " << argv[2] << std::endl;
    return 1;
  }

  // // Convert cv::Mat to ImageFrame
  // auto input_frame = std::make_unique<ImageFrame>(ImageFormat::SRGB, image.cols, image.rows);
  // cv::Mat input_mat = formats::MatView(input_frame.get());
  // image.copyTo(input_mat);

  // Create the graph
  CalculatorGraph graph;
  MP_RETURN_IF_ERROR(graph.Initialize(config));
  MP_ASSIGN_OR_RETURN(OutputStreamPoller poller,
                      graph.AddOutputStreamPoller("out"));
  MP_RETURN_IF_ERROR(graph.StartRun({}));


  // Add the image to the graph
  status = graph.InputSidePackets(
    "input_video_path", Adopt(input_frame.release()).At(Timestamp(0)));
  if (!status.ok()) {
    std::cerr << "Failed to add packet to input stream: " << status.message() << std::endl;
    return 1;
  }

  // // Run the graph
  // status = graph.StartRun({});
  // if (!status.ok()) {
  //   std::cerr << "Failed to start graph run: " << status.message() << std::endl;
  //   return 1;
  // }

  // // Get the output
  // std::vector<Packet> output_packets;
  // status = graph.WaitUntilIdle(&output_packets);
  // if(!status.ok()) {
  //   std::cerr << "Failed to wait for graph output: " <<status.message() << std::endl;
  //   return 1;
  // }



  // Process the output
  if (!output_packets.empty()) {
    const auto& keypoints_packet = output_packets[0];
    const auto& keypoints = keypoints_packet.Get<std::vector<cv::KeyPoint>>();
    std::cout << "Detected " << keypoints.size() << " keypoints." << std::endl;
    
    for (const auto& kp : keypoints) {
      std::cout << "Keypoint at (" << kp.pt.x << ", " << kp.pt.y << "), size: " << kp.size << std::endl;
    }
  } else {
    std::cout << "No output packets received." << std::endl;
  }

  // Close the graph
  status = graph.CloseInputStream("input_image");
  if (!status.ok()) {
    std::cerr << "Failed to close input stream: " << status.message() << std::endl;
    return 1;
  }

  status = graph.WaitUntilDone();
  if (!status.ok()) {
    std::cerr << "Failed to wait for graph completion: " << status.message() << std::endl;
    return 1;
  }

  return 0;
}

}  // namespace mediapipe

int main(int argc, char** argv) {
  return mediapipe::main(argc, argv);
}