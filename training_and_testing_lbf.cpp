/*
@author : kushashwaravishrimali (github.com/krshrimali), kushashwaravishrimali@gmail.com
Function: Training and Testing FacemarkLBF model on helen dataset (www.ifp.illinois.edu/~vuongle2/helen/)

Usage: ./output_file 

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
CascadeClassifier face_cascade("../Task-3/haarcascade_frontalface_alt.xml");
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
    params.model_filename = "helenLBF.model"; // the trained model will be saved using this filename
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
    cv::face::loadDatasetList("trainset/helen_train.txt", "trainset/helen_landmarks.txt", images_train, landmarks_train);
    
    Mat image;
    std::vector<Point2f> facial_points;

    if(params.cascade_face.empty()) {
        std::cout << "Face detector not provided. Recheck" << std::endl;
        // exit
        return 0;
    }

    for(size_t i=0;i<images_train.size();i++){
        std::cout << images_train[i].c_str() << std::endl;
        image = cv::imread(images_train[0]);

        if(image.empty()) {
            std::cout << "Image not read successfully. Please check the paths provided" << std::endl;
            return 0;
        }

        // load face points in each .pts file to facial_points
        cv::face::loadFacePoints(landmarks_train[i],facial_points);

        // add the training sample from facial_points
        facemark->addTrainingSample(image, facial_points);
    }

    // training algorithm facemarkLBF
    facemark->training();

    /* testing of the trained model */
    // load trained model
    facemark->loadModel("helenLBF.model");

    // load a sample test image
    Mat image2 = imread("helen/testset/296814969_3.jpg");

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
        for(int i = 0; i < landmarks.size(); i++) {
            drawLandmarks(image2, landmarks[i]);
        }
    }
    cv::imshow("Landmarks Detection", image2);
    cv::waitKey(0);
    cv::destroyAllWindows();
}