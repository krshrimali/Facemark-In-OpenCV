#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include "../FacemarkLBF/drawLandmarks.hpp"

using namespace std;
using namespace cv;
using namespace cv::face;

CascadeClassifier face_cascade;
bool myDetector( InputArray image, OutputArray ROIs );
bool myDetector( InputArray image, OutputArray ROIs ){
    Mat gray;
    vector<Rect> faces;
    if(image.channels()>1){
        cvtColor(image.getMat(),gray,CV_BGR2GRAY);
    }
    else{
        gray = image.getMat().clone();
    }
    equalizeHist( gray, gray );
    face_cascade.detectMultiScale( gray, faces, 1.1, 3,0, Size(30, 30) );
    Mat(faces).copyTo(ROIs);
    return true;
}


int main(int argc, char** argv) {
    // Load Face Detector
    // CascadeClassifier faceDetector("../haarcascade_frontalface_alt.xml");
    String cascade_name = "../haarcascade_frontalface_alt.xml";

    face_cascade.load(cascade_name);
    
    
    // Create instance of Facemark
    Ptr<FacemarkKazemi> facemark = FacemarkKazemi::create();
    // facemark->setFaceDetector(myDetector);    
    facemark->loadModel("model_new.dat");

    // Load landmark detector
    // VideoCapture cam(0);
    Mat frame = imread("Me.jpg", 1);
    // Variable to store video frame, and it's grayscale
    // Mat frame, gray;
    vector< vector<Point2f> > landmarks;
    vector< Rect> faces;
    // while(cam.read(frame)) {
        // Find face
        // Resize frame first to (600, 600)
    faces.clear();
    landmarks.clear();
    resize(frame,frame,Size(600,600),0,0,INTER_LINEAR_EXACT);
    Mat gray;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    
    // faceDetector.detectMultiScale(gray, faces);
    // facemark->getFaces(frame, faces);
    face_cascade.detectMultiScale(gray, faces);
    cout << faces.size() << endl;
    
    if(faces.size()==0){
        cout<<"No faces found in this frame"<<endl;
    }

    else{
        for( size_t i = 0; i < faces.size(); i++ )
        {
            cv::rectangle(frame,faces[i],Scalar( 255, 0, 0 ));
        }
        if(facemark->fit(frame,faces,landmarks))
        {
            for(unsigned long i=0;i<faces.size();i++){
                for(unsigned long k=0;k<landmarks[i].size();k++)
                    cv::circle(frame,landmarks[i][k],3,cv::Scalar(0,0,255),FILLED);
            }
        }
    }
    namedWindow("Detected_shape");
    imshow("Detected_shape",frame);
    // if(waitKey(1) >= 0) return 1;
    waitKey(0);
    destroyAllWindows();
    bool success = facemark->fit(frame, faces, landmarks);
    cout << success << endl;
    if(success)
    {
       for(int i = 0; i < landmarks.size(); i++) {
        //   cout << landmarks[i] << endl;
          drawLandmarks(frame, landmarks[i]);
       }
       cout << "Landmarks done" << endl;
    }
    
    imshow("Landmarks Detection", frame);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
