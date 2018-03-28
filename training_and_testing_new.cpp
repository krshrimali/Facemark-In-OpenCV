/*
@author : kushashwaravishrimali (github.com/krshrimali), kushashwaravishrimali@gmail.com
Function: Training and Testing FacemarkLBF model on helen dataset (www.ifp.illinois.edu/~vuongle2/helen/)

Usage: ./output_file train OR ./output_file test
(use train if you want to train the model, use test if you want to test the model)

Data: make sure you have helen_train.txt and helen_landmarks.txt in trainset folder.

Steps to generate helen_train.txt and helen_landmarks.txt
1) Put images (.jpg) and (.pts) in data/ folder, and execute following commands on terminal
2) ls $PWD/data/*.jpg >> trainset/helen_train.txt
3) ls $PWD/data/*.pts >> trainset/helen_landmarks.txt

For output image: https://github.com/krshrimali/Facemark---OpenCV/
*/

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include "FacemarkLBF/drawLandmarks.hpp"
#include <opencv2/face/facemark.hpp>
#include <opencv2/face/facemarkLBF.hpp>
#include "FacemarkLBF/drawLandmarks.hpp"

#include "opencv2/objdetect/objdetect.hpp"
#include <iostream>

using namespace std;

// function declaration - user defined
bool myDetector( InputArray image, OutputArray ROIs, void* M = 0 );

// declaring global variables
CascadeClassifier face_cascade("lbpcascade_frontalface.xml");
cv::face::FacemarkLBF::Params params;
bool myDetector( InputArray image, OutputArray ROIs, void* M){
    /*
    Function is user-defined faceDetector utility function. Alternate to default face detectors.
    Arguments: InputArray image, OutputArray ROIs, void* M (default 0)
    Return Type: boolean (true if successful, else false) 
    */

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

int main(int argc, char** argv) {
    cv::face::FacemarkLBF::Params params;
    // cascade xml file - faceDetector
    params.cascade_face = "lbpcascade_frontalface.xml";
    params.model_filename = "helen_new.model"; // the trained model will be saved using this filename
    Ptr<cv::face::Facemark> facemark = cv::face::FacemarkLBF::create(params);
    
    facemark->setFaceDetector(myDetector); // we must guarantee proper lifetime of "config" object

    params.save_model = 1; // save the trained model or not? 
    params.n_landmarks = 68; // number of landmark points
    params.initShape_n = 10; // number of multiplier for make data augmentation
    params.stages_n=5; // amount of refinement stages
    params.tree_n=6; // number of tree in the model for each landmark point
    params.tree_depth=5; //he depth of decision tree
    
    std::vector<String> images_train;
    std::vector<String> landmarks_train;

    std::cout << images_train.size() << std::endl;
    // images_train, landmarks_train
    cv::face::loadDatasetList("trainset/helen_train_.txt", "trainset/helen_points_.txt", images_train, landmarks_train);
    std::cout << images_train.size() << std::endl;    
    std::cout << landmarks_train.size() << std::endl;    
    Mat image, image_mirror;
    std::vector<Point2f> facial_points;
/*
    if(params.cascade_face.empty()) {
        std::cout << "Face detector not provided. Recheck" << std::endl;
        // exit
        return 0;
    }

    // cout << argv[1] << endl;
    cout << "Check: " << images_train.size() << endl;
    // if(strncmp(argv[1], "train", 4)) {
    cout << "Starting loading face points..." << endl;
    for(size_t i=0,j=0;i<100;i+=2,j++){
      //if(find('mirrored', image_train[i].c_str())) {
          //for(int j = 0; j < 
          //
        std::cout << "First image: " << images_train[i].c_str() << std::endl;
        std::cout << "Second Image: " << images_train[i+1].c_str() << std::endl;
        image = cv::imread(images_train[i]);
        image_mirror = cv::imread(images_train[i+1]);
        if(image.empty() || image_mirror.empty()) {
            std::cout << "Image not read successfully. Please check the paths provided" << std::endl;
            return 0;
        }

        // load face points in each .pts file to facial_points
        cout << landmarks_train[j].c_str() << endl;
        cv::face::loadFacePoints(landmarks_train[j],facial_points);
        // add the training sample from facial_points
        facemark->addTrainingSample(image, facial_points);
        facemark->addTrainingSample(image_mirror, facial_points);
    }*/

    // training algorithm facemarkLBF
    // facemark->training();
    // else if(strncmp(argv[1], "test", 4)) {
    /* testing of the trained model */
    // load trained model

    facemark->loadModel("helen_new.model");

    // load a sample test image
    Mat image2 = imread("Me.jpg");

    // if wrong path provided or image is corrupted
    if(image2.empty()) {
        cout << "Empty" << endl;
        return 0;
    }

    /* the faces */
    vector<Rect> faces;
    facemark->getFaces(image2, faces);

    // perform fitting process
    vector<vector <Point2f> > landmarks;
    bool success = facemark->fit(image2, faces, landmarks);

    // if fitting is successful
    if(success) {
      cout << "Success" << endl;
        for(int i = 0; i < landmarks.size(); i++) {
            drawLandmarks(image2, landmarks[i]);
        }
    }
    cv::imwrite("image2.jpg", image2);
    /*
    cv::imshow("Landmarks Detection", image2);
    cv::waitKey(0);
    cv::destroyAllWindows();
    */
}
