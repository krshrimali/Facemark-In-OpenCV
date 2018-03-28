#!/usr/bin/python
import os
import sys
import cv2
import glob

# utility function 
# strips version, landmark points and curly brackets from .pts files
# to convert for this program
def convert_pts(file_, file_new):
    print(file_)
    with open(file_, 'r') as f_:
        list_ = f_.readlines()
    f_.close()

    with open(file_new, 'w') as f:
        for item in list_[3:-1]:
            f.write("%s" % item)
    f.close()
# convert_pts("2109755281_4.pts", "2109755281_4_new.pts")
# These indices start from 1 instead of 0.
# # just helen194
mirrorIndices = [
   17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
   26, 25, 24, 23, 22, 21, 20, 19, 18,
   36, 35, 34, 33, 38, 37,
   30, 29, 28, 27, 32, 31,
   45, 44, 43, 42, 41, 40, 39, 50, 49, 48, 47, 46,
   55, 54, 53, 52, 51, 58, 57, 56,
   # 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
<<<<<<< HEAD
   # 69, 70, 
=======
>>>>>>> f32e8cfa5a1cd578c95a896d3ecca83c2423adf5
   59, 60, 61, 62, 63, 64, 65, 66, 67, 68
 ]

# # iBUG 300W and eyebrows from Helen194
# mirrorIndices = [
#   17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
#   18, 19, 20, 21, 26, 25, 24, 23, 22,
#   36, 35, 34, 33, 38, 37,
#   30, 29, 28, 27, 32, 31,
#   45, 44, 43, 42, 41, 40, 39, 50, 49, 48, 47, 46,
#   55, 54, 53, 52, 51, 58, 57, 56,
#   69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
#   59, 60, 61, 62, 63, 64, 65, 66, 67, 68
# ]

# iBUG 300W and eyebrows
'''
mirrorIndices = [
  # jaw line
  # 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 # original
  17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, # mirror
  # upper eyebrows
  # 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 # original
  27, 26, 25, 24, 23, 22, 21, 20, 19, 18,  # mirror
  # nose
  # 28, 29, 30, 31, 32, 33, 34, 35, 36 # original
  28, 29, 30, 31, 36, 35, 34, 33, 32, # mirror
  # eyes
  # 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48 # original
  46, 45, 44, 43, 48, 47, 40, 39, 38, 37, 42, 41, # mirror
  # outer lips
  # 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60 # original
  55, 54, 53, 52, 51, 50, 49, 60, 59, 58, 57, 56, # mirror
  # inner lips
  # 61, 62, 63, 64, 65, 66, 67, 68 # original
  65, 64, 63, 62, 61, 68, 67, 66, # mirror
  # lower eyebrows
  # 69, 70, 71, 72, 73, 74, 75, 76, 77, 78 # orignal
  78, 77, 76, 75, 74, 73, 72, 71, 70, 69 # mirror
]
'''

# facial landmark data directory
fldDatadir = sys.argv[1]
numPoints = sys.argv[2]
<<<<<<< HEAD
# suffix = '_bv' + '70'
suffix = ''
=======
suffix = '_bv' + numPoints
# suffix = ''
>>>>>>> f32e8cfa5a1cd578c95a896d3ecca83c2423adf5
# helenTestDir = os.path.join(fldDatadir, 'test')
# helenTrainDir = os.path.join(fldDatadir, 'train')
print(fldDatadir)
rectPaths = glob.glob(os.path.join(fldDatadir, '*.jpg'))
print(rectPaths)
imageNames = [os.path.splitext(x)[0] for x in rectPaths if 'mirror' not in x]
'''for i, string in enumerate(imageNames):
    imageNames[i] = string.replace("_rect","")
    print(imageNames[i])
    '''

# print(imageNames)
numImages = len(imageNames)
print(numImages)
count = 0
# Iterate over all image names
for n, imageName in enumerate(imageNames):
  # # Test for small number of files
<<<<<<< HEAD
  # if count > 5:
  #  break
=======
  if count > 5:
    break
>>>>>>> f32e8cfa5a1cd578c95a896d3ecca83c2423adf5

  if 'mirror' not in imageName:
    # print('{}/{} - {}'.format(n+1, numImages, imageName))
    # print(imageName)
    # path to image
    imagePath = imageName+'.jpg'
    im = cv2.imread(imagePath, cv2.IMREAD_COLOR)
    if im is None:
      print('Unable to read image:{}'.format(imagePath))
      continue
    else:
      h, w, ch = im.shape
      count += 1

      # mirror image
      # write mirrored image to disk
      mirrorImagePath = imageName+'_mirror.jpg'
      if not os.path.exists(mirrorImagePath):
        imMirror = cv2.flip(im, 1)
        cv2.imwrite(mirrorImagePath, imMirror)

      # mirror bounding box
      # read rectangle file corresponding to image
      rectFile = imageName + '_rect.txt'
      with open(rectFile, 'r') as file:
        rect = file.readline().strip()
      rect = rect.split()
      left, top, width, height = rect[0:4]
      print(left, top, width, height)
      # calculate coordinates of mirrored bounding box
      leftMirrored = int(w - (int(left) + int(width)))
      rectMirrored = ' '.join([str(leftMirrored), top, width, height])
      # write mirrored bounding box to disk
      mirrorRectFile = imageName + '_mirror_rect.txt'
      print(mirrorRectFile)
      with open(mirrorRectFile, 'w+') as h:
        h.write(rectMirrored + '\n')

      # mirror landmarks
<<<<<<< HEAD
      pointsPath = imageName + suffix + '.pts'
      pointsMirrorPath = imageName + '_mirror' + '.txt'
      
      convert_pts(pointsPath, imageName + suffix + "_new.pts")
      # convert_pts(pointsPath)
=======
      pointsPath = imageName + suffix + '.txt'
      pointsMirrorPath = imageName + '_mirror' + '.txt'
      
>>>>>>> f32e8cfa5a1cd578c95a896d3ecca83c2423adf5
      # Check if path to annotation file exists
      if os.path.exists(pointsPath):
        # open file
        with open(pointsPath, 'r') as f:
<<<<<<< HEAD
            print(pointsPath)
            if(True):
                # read all lines
                points = [x.strip() for x in f.readlines()]
                print(points)
=======
            if(f.readlines()):
                # read all lines
                points = [x.strip() for x in f.readlines()]
>>>>>>> f32e8cfa5a1cd578c95a896d3ecca83c2423adf5
        mirrorPoints = []
        # print(points)
        for point in points:
          x1, y1 = point.split(' ')
          x1Mirror = '{0:.2f}'.format(w - float(x1))
          mirrorPoints.append(x1Mirror + ' ' + y1 + '\n')
        print(mirrorPoints)
        mirrorPointsRearranged = []
        for idx in mirrorIndices:
          mirrorPointsRearranged.append(mirrorPoints[idx-1])
        with open(pointsMirrorPath, 'r+') as h:
          h.writelines(mirrorPointsRearranged)
      else:
        print('Unable to find path:{}'.format(pointsPath))
