import os
import cv2
import random


# define scale so that points can be printed well
scale = 5

def draw_rect(im, bbox):
  x1, y1, x2, y2 = bbox
  cv2.rectangle(im, (x1, y1), (x2, y2), (0, 255, 255), thickness=5, lineType=cv2.LINE_8)


def draw_landmarks(im, parts, color=(0, 0, 255), radius=2, filled=1):
  for i, part in enumerate(parts):
    # print shape.num_parts()
    px, py = part
    cv2.circle(im, (px, py), filled, color, thickness=radius, lineType=cv2.LINE_AA)
    cv2.putText(im, str(i+1), (px, py), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 200, 100), 4)

fldDataDir = "D:\\work\\bigVision\\eyebrows\\helen300W"
image_name = "296814969_3_mirror"

image_path = os.path.join(fldDataDir, 'train', image_name + '.jpg')

if os.path.exists(image_path):
  print('image found in train directory')
else:
  image_path = os.path.join(fldDataDir, 'test', image_name + '.jpg')
  if os.path.exists(image_path):
    print('image found in test directory')
  else:
    print('Error! image not found in both train/test. {}'.format(image_name))

if not os.path.exists(image_path):
	print("image not found {}".format(image_path))

im = cv2.imread(image_path, cv2.IMREAD_COLOR)
im = cv2.resize(im, (0, 0), fx=scale, fy=scale)

# rect_name = os.path.splitext(image_path)[0] + '_rect.txt'

# with open(rect_name) as f:
#   line = f.readline()
#   left, top, width, height = [float(n) for n in line.strip().split()]
#   right = left + width
#   bottom = top + height
# x1, y1, x2, y2 = int(scale*left), int(scale*top), int(scale*right), int(scale*bottom)
# bbox = [x1, y1, x2, y2]

# draw 78
points_name = os.path.splitext(image_path)[0] + '_bv78.txt'

parts = []
with open(points_name) as g:
  lines = [x.strip() for x in g.readlines()]
  for line in lines:
    left, right = [float(n) for n in line.split()]
    px, py = int(scale*left), int(scale*right)
    parts.append([px, py])

# draw_rect(im, bbox)
draw_landmarks(im, parts, color=(0, 0, 255), radius=2, filled=1)

# # draw 194
# points_name = os.path.splitext(image_path)[0] + '_helen194.txt'

# parts = []
# with open(points_name) as g:
#   lines = [x.strip() for x in g.readlines()]
#   for line in lines:
#     left, right = [float(n) for n in line.split()]
#     px, py = int(scale*left), int(scale*right)
#     parts.append([px, py])
# draw_landmarks(im, parts, color=(255, 0, 0), radius=5, filled=0)

output_image_path = os.path.join("D:\\work\\bigVision\\eyebrows", image_name + '_300W.jpg')
cv2.imwrite(output_image_path, im)
