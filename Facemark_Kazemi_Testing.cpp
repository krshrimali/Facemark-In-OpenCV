#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include "../FacemarkLBF/drawLandmarks.hpp"

using namespace std;
using namespace cv;
using namespace cv::face;

int main(int argc, char** argv) {
    // Load Face Detector
    CascadeClassifier faceDetector("../FacemarkLBF/haarcascade_frontalface_alt2.xml");

    // Create instance of Facemark
    Ptr<FacemarkKazemi> facemark = FacemarkKazemi::create();
    
    facemark->loadModel("model.dat");

    // Load landmark detector
    VideoCapture cam(0);

    // Variable to store video frame, and it's grayscale
    Mat frame, gray;

    while(cam.read(frame)) {
        // Find face
        vector<Rect> faces;

        cvtColor(frame, gray, COLOR_BGR2GRAY);

        faceDetector.detectMultiScale(gray, faces);
        cout << faces.size() << endl;
        vector< vector<Point2f> > landmarks;

        bool success = facemark->fit(frame, faces, landmarks);

        if(success)
        {
           for(int i = 0; i < landmarks.size(); i++) {
            //   cout << landmarks[i] << endl;
              drawLandmarks(frame, landmarks[i]);
           }
        }
        
        imshow("Landmarks Detection", frame);
        if(waitKey(1) == 27) break;
    }
    return 0;
}
