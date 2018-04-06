#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/objdetect.hpp"
#include "../FacemarkLBF/drawLandmarks.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace cv;
using namespace cv::face;

static bool myDetector(InputArray image, OutputArray faces, CascadeClassifier *face_cascade)
{
    Mat gray;

    if (image.channels() > 1)
        cvtColor(image, gray, COLOR_BGR2GRAY);
    else
        gray = image.getMat().clone();

    equalizeHist(gray, gray);

    vector<Rect> faces_;
    face_cascade->detectMultiScale(gray, faces_, 1.4, 2, CASCADE_SCALE_IMAGE, Size(30, 30));
    Mat(faces_).copyTo(faces);
    return true;
}

int main(int argc, char** argv) {
    string annotations = "../train_new_set/helen_pts.txt";
    string imagesList = "../train_new_set/helen_img.txt";
    string configfile_name = "../sample_config_file.xml";
    string modelfile_name = "model_new.dat";
    string cascade_name = "../haarcascade_frontalface_alt.xml";
    Size scale(460, 460);

    CascadeClassifier face_cascade;
    face_cascade.load(cascade_name);
    FacemarkKazemi::Params params;
    params.configfile = configfile_name;
    Ptr<FacemarkKazemi> facemark = FacemarkKazemi::create(params);
    facemark->setFaceDetector((FN_FaceDetector)myDetector, &face_cascade);

    std::vector<String> images;
    std::vector<std::vector<Point2f> > facePoints;
    loadTrainingData(imagesList, annotations, images, facePoints, 0.0);

    vector<Mat> Trainimages;
    std::vector<std::vector<Point2f> > Trainlandmarks;

    Mat src;
    for (unsigned long i = 0; i < images.size(); i++) {
        src = imread(images.at(i));
        std::cout << "Image " <<  i << " " << src.rows << " " << src.cols << endl;

        if (src.empty()) {
            cout << images.at(i) << endl;
            cerr << string("Image not found\n.Aborting...") << endl;
            continue;
        }

        std::cout << "Annotation " << i << " " << facePoints.at(i).size() << endl;

        Trainimages.push_back(src);
        Trainlandmarks.push_back(facePoints.at(i));
    }
    cout << "Got data" << endl;

    facemark->training(Trainimages, Trainlandmarks, configfile_name, scale, modelfile_name);
    cout << "Training complete" << endl;
    
    // facemark->loadModel("model.dat");
    // cout << "Model loaded" << endl;
    // // load a sample test image
    // Mat image2 = imread("../ibug/image_003_1.jpg");
    // cout << "Image loaded" << endl;
    // // if wrong path provided or image is corrupted
    // if(image2.empty()) {
    //     cout << "Empty" << endl;
    //     return 0;
    // }

    // Mat gray;
    // cvtColor(image2, gray, COLOR_BGR2GRAY);

    // CascadeClassifier faceDetector("../FacemarkLBF/haarcascade_frontalface_alt2.xml");
    // vector<Rect> rects;
    // faceDetector.detectMultiScale(gray, rects);
    // cout << rects.size() << endl;
    // for(int j=0;j<rects.size();j++){
    //     cout << rects[j] << endl;
    //     cv::rectangle(image2, rects[j], cv::Scalar(255,0,255));
    // }  
    // imshow("image2", image2);
    // waitKey(0);
    // destroyAllWindows();
    // cout << rects[10] << endl;
    // /* the faces */
    // // facemark->getFaces(image2, rects);
    // // cout << rects[1] << endl;
    // // cout << faces[0] << endl;
    // // perform fitting process
    // vector<vector <Point2f> > landmarks;
    // bool success = facemark->fit(image2, rects, landmarks);
    // cout << success << endl;
    
    // // if fitting is successful
    // if(success) {
    //     for(int i = 0; i < landmarks.size(); i++) {
    //         drawLandmarks(image2, landmarks[i]);
    //     }
    // }
    // cv::imshow("Landmarks Detection", image2);
    // cv::waitKey(0);
    // cv::destroyAllWindows();
    // return 0;
}
