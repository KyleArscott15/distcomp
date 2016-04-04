#!/bin/bash

FINAL_FILE=images/output.mp4
INTRO_FILE=intro.mp4
INTRO_IMG=title_slide2.jpg
FPS=1           # 30 frames per second, final should be 30
RES=400x400      # should be 1920x1080
 
#===========================================================

# convert all *.bmp into .jpg
for f in images/*.bmp;
do
	convert $f $f.jpg
	rm $f
done

# turn intro slide into a video
# ffmpeg -loop 1 -i $INTRO_IMG -t 5 $INTRO_FILE

# make video from the images
ffmpeg -r $FPS -i images/image-%03d.bmp.jpg -b 5000k -s $RES $FINAL_FILE

# combine intro + main video

# overlay audio track
#ffmpeg -i output.mp4 -i ../Kamelot_Insomnia.mp3 -filter_complex " [1:0] apad " -shortest final.mp4
# WORKS BEST ffmpeg -i output.mp4 -i ../Kamelot_Insomnia.mp3 -codec copy -shortest final.mp4
# ffmpeg -i videofile.mp4 -i audiofile.wav -filter_complex " [1:0] apad " -shortest output.mp4
#ffmpeg -i ../Kamelot_Insomnia.mp3 -i $FILE -acodec copy -vcodec copy -f avi $FILE.avi
#ffmpeg -i ../Kamelot_Insomnia.mp3 -i $FILE -acodec copy -vcodec copy -f avi $FILE.avi

