# Facemark-In-OpenCV

Adding progress details, proper debugs and more.

Progress
## Task Facemark :

Details - Train FacemarkKazemi and FacemarkAAM models.

Stage - 1 : FacemarkLBF
1.a) Experiment - 1 : Train FacemarkLBF, learn and try.

Getting used to Facemarking. About it : tutorials (https://github.com/opencv/opencv_contrib/tree/master/modules/face/tutorials/facemark_usage and more, refer REFERENCES.MD)

1.b) Exerpiment - 2 : Use the given LBFModel.yml file and check the outputs. Found *accurate*. 

1.c) Experiment - 3 : Trained my own model, verified results. Found *accurate*.

Stage - 2 : FacemarkKazemi
2.a) Experiment - 1 : Training FacemarkKazemi Model. Problems faced.
  2.a).i) - Configuration File? [http://answers.opencv.org/question/187513/configuration-configxml-file-facemarkkazemi-training/][Question Asked : Configuration File, OpenCV Forum] (Level : Easy)
            Solution : https://github.com/opencv/opencv_contrib/blob/master/modules/face/samples/sample_config_file.xml [sample_config_file.xml]
  
  2.a).ii) - Why result poor? On discussion with Vaibhaw, good point here:
          "1. use one of the training images as query image i.e. predict facemark on one of the train images using modelif the prediction is good, it means model is trained well on training set but not generalized enoughwe have to add more dataif the prediction is not good, something is wrong with training or you will have to tune training parameters"
    
 2.a).iii) Need of more data to train. Vaibhaw's response on Satya Sir's request for the links:
 
      "You can download the Dlib's face landmark data here - http://dlib.net/files/data/ibug_300W_large_face_landmark_dataset.tar.gz which is a larger dataset than Helen.

      I am attaching a Python script (generateMirrorAnnotations.py) that you can use to generate mirrored images and annotations. You will have to modify this script as this was written for a slightly different dataset.
      This script was written for a database which has following structure:
      1. All the images and their corresponding annotation files are kept in the same folder
      2. Each image has 2 annotation files: facial landmarks and face rectangle/bounding box
      2a. if the image name is image_001.jpg its face landmark file's name is image_001_bv78c.txt and 
      2b. face rectangle file's name is image_001_rect.txt
      3. face rectangle files has 4 values separated by space: left, top, width, height i.e. x1, y1, w, h
      4. facial landmark file has x, y coordinates of each point separated by space on each line

      You need to modify:
      1. mirrorIndices variable - such that it provides a correct mapping for your dataset when an image is vertically flipped. Helen and iBUG300W have a different order of facial landmarks.
      Important: Index numbers written in scripts start from 1 instead of 0 i.e. landmark point numbers are from 1 to 78 instead of 0 to 77. I did so because then it becomes much easier to write indices or mirror points.
      2. name of annotation files

      You can modify drawLandmarks scripts to test whether the mapping you have written is correct or you can use code from these scripts to write your own script.
      
      OR
      You can download a dataset where we have already generated the mirrored images and annotations here https://drive.google.com/open?id=0B3wncKrtnPc_YmlCRENQRzU4UFk
      "
  2.a.iv) OpenCV 3.4.1 Required for Facemarking, installed on the server.
  
  2.a.v)  Task on editing \_bv70.txt files :
          Adding line : version : 1, n_points : 68 and braces. (As given in the tutorial)
          Used Python modules for this, appending by opening the file.
          Set seek(0) when you want to add in the beginning. (prepending)
          Also, reduce 70 points to 68 points by deleting last two points.
  
  2.a.vi) Debugging detector function : Increase nearest point neighbors from 3 to 5 (anything), and check parameters. Or       keep it as it is. 
  
          Draw output points from detector function, draw faces and verify. (VERIFIED)
          Draw landmarks points recorded in .pts / *[_bv70].txt files 

  2.a.vii) Relative shapes error : 
  `
  OpenCV(3.4.1-dev) Error: Bad argument (Error while finding relative shape. Aborting....) in getRelativePixels, file /data/kushashwa/Install/opencv_contrib/modules/face/src/trainFacemark.cpp, line 125

TBB Warning: Exact exception propagation is requested by application but the linked library is built without support for it

terminate called after throwing an instance of 'tbb::captured_exception'

  what():  OpenCV(3.4.1-dev) /data/kushashwa/Install/opencv_contrib/modules/face/src/trainFacemark.cpp:125: error: (-5) Bad argument: Error while finding relative shape. Aborting.... in function getRelativePixels`

Solution : Looked like some thing is wrong with the points file. There was one new line added before a point in .txt file, by fault. Removed that. And it worked!

2.b) Experiment - 2 : Model trained successfully. Results - better than before. Can not be called poor though. *Trained with 3.5k images*. One more points file is broken most probably, as was giving segmentation fault. 


`Personal Notes : 
scp -r ubuntu@76.176.29.4:/data/kushashwa/Task_Facemark/facial_landmark_data/* . 
scp file_name ubuntu@76.176.29.4:/data/kushashwa/Task_Facemark/file_new_name
`
