#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include "FacemarkLBF/drawLandmarks.hpp"
#include <opencv2/face/facemark.hpp>
#include <opencv2/face/facemarkLBF.hpp>
// #include <opencv2/highgui.h>

// Inheritance diagram for cv::face::FacemarkLBF:

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;

// using namespace cv;
// using namespace cv::face;
cv::face::FacemarkLBF::Params params;

CascadeClassifier face_cascade;
bool myDetector( InputArray image, OutputArray ROIs, void* M );
bool myDetector( InputArray image, OutputArray ROIs, void* M ){
    Mat gray;
    std::vector<Rect> faces;
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

// struct Conf {
//     cv::String model_path;
//     double scaleFactor;
//     cv::CascadeClassifier face_detector;

//     Conf(cv::String s, double d){
//         model_path = s;
//         scaleFactor = d;
//         face_detector.load(model_path); 
//     };
//     // String face_cascade_name = "haarcascade_frontalface_alt.xml";
//     // String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
//     // cv::CascadeClassifier face_cascade;
//     // cv::CascadeClassifier eyes_cascade;
// };

// void detectAndDisplay( Mat frame )
// {
//     std::vector<Rect> faces;
//     Mat frame_gray;
//     cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
//     equalizeHist( frame_gray, frame_gray );
//     //-- Detect faces
//     face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(60, 60) );
//     for ( size_t i = 0; i < faces.size(); i++ )
//     {
//         Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
//         ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
//         Mat faceROI = frame_gray( faces[i] );
//         std::vector<Rect> eyes;
//         //-- In each face, detect eyes
//         eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
//         for ( size_t j = 0; j < eyes.size(); j++ )
//         {
//             Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
//             int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
//             circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
//         }
//     }
//     //-- Show what you got
//     // imshow( window_name, frame );
// }

// bool myDetector(InputArray image, OutputArray faces, void* config = 0){
//     // Mat gray;
//     // if (image.channels() > 1)
//     //     cvtColor(image, gray, COLOR_BGR2GRAY);
//     // else
//     //     gray = image.getMat().clone();
//     // equalizeHist(gray, gray);
//     // std::vector<Rect> faces_;
//     cv::CascadeClassifier face_cascade;
//     face_cascade.load("lbpcascade_frontalface.xml");
//     // face_detector.detectMultiScale(gray, faces_, 1.4, 2, CASCADE_SCALE_IMAGE, Size(30, 30) );
//     // Mat(faces_).copyTo(faces);
//     // return true;
//     Mat gray;
//     std::vector<Rect> & faces_ = *(std::vector<Rect>*) faces.getObj();
//     faces_.clear();

//     if(config!=0){
//         //do nothing
//     }

//     if(image.channels()>1){
//         cv::cvtColor(image.getMat(),gray,CV_BGR2GRAY);
//     }else{
//         gray = image.getMat().clone();
//     }
//     cv::equalizeHist( gray, gray );

//     face_cascade.detectMultiScale( gray, faces_, 1.4, 2, CV_HAAR_SCALE_IMAGE, Size(30, 30) );

//     return true;
// }

int main(int argc, char** argv) {
    // cv::CascadeClassifier faceDetector("../Task-3/haarcascade_frontalface_alt.xml");
    // struct Conf config("../Task-3/lbpcascade_frontalface.xml", 1.4);
    // String face_cascade

    
    // params.set_values("facemark_ibug.model", 68, 5, 6, 5);
    cv::face::FacemarkLBF::Params params;
    params.model_filename = "sampleLBF.model"; // the trained model will be saved using this filename
    Ptr<cv::face::Facemark> facemark = cv::face::FacemarkLBF::create(params);
    // std::cout << &config << std::endl;
    facemark->setFaceDetector(myDetector); // we must guarantee proper lifetime of "config" object


    params.n_landmarks = 68; // number of landmark points
    params.initShape_n = 10; // number of multiplier for make data augmentation
    params.stages_n=5; // amount of refinement stages
    params.tree_n=6; // number of tree in the model for each landmark point
    params.tree_depth=5; //he depth of decision tree
    // facemark = cv::face::FacemarkLBF::create(params);
    
    
    std::vector<String> images_train;
    std::vector<String> landmarks_train;
    // std::vector<std::vector<Point2f> > landmarks_train;
    std::cout << images_train.size() << std::endl;
    // images_train, landmarks_train
    cv::face::loadDatasetList("trainset/images_train.txt", "trainset/annotation_train.txt", images_train, landmarks_train);
    
    std::cout << images_train.size() << std::endl;
    std::cout << landmarks_train.size() << std::endl;
    // std::cout << landmarks_train[0] << std::endl;
    Mat image;
    std::vector<Point2f> facial_points;
    
    // // Extra
    // String filename = landmarks_train[0];
    // std::string line, item;
    // std::ifstream infile(filename.c_str());

    /*pop the version*/
    // std::getline(infile, line);

    // std::string line2;
    // std::ifstream infile2(line.c_str());
    // std::getline(infile2, line2);

    // std::cout << "line ho rhi hai " << line2 << std::endl;
    // CV_Assert(line.compare(0,7,"version")==0);

    /* newest */
    // std::vector<String> imagenames;
    // std::vector< vector<Point2f> > trainlandmarks,Trainlandmarks;
    // std::vector<Mat> trainimages;

    // cv::face::loadTrainingData("trainset/annotation_train.txt",trainlandmarks,imagenames);
    
    // std::cout << trainlandmarks[0] << std::endl;
    for(size_t i=0;i<images_train.size();i++){
        std::cout << images_train[i].c_str() << std::endl;
        image = cv::imread(images_train[0]);
        // cv::imshow("image", image);
        // cv::waitKey(0);
        // cv::destroyAllWindows();
        std::cout << landmarks_train[i] << std::endl;
        cv::face::loadFacePoints(landmarks_train[i],facial_points);
        facemark->addTrainingSample(image, facial_points);
    }
    /*train the Algorithm*/
    facemark->training();
    facemark->loadModel("sampleLBF.model");

    Mat image2 = imread("Me.jpg");
    if(image2.empty()) {
        cout << "Empty" << endl;
        return 0;
    }
    /* the faces */
    vector<Rect> faces;
    facemark->getFaces(image2, faces);

    // perform fitting process
    vector<vector <Point2f> > landmarks;
    facemark->fit(image2, faces, landmarks);

    cv::imshow("faces", faces);
    cv::imshow("image", image2);
    cv::waitKey(0);
    cv::destroyAllWindows();
}