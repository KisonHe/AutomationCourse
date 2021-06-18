import vlc
import time
import argparse


parser = argparse.ArgumentParser()

parser.add_argument('--num', type=int,required=True)
args = parser.parse_args()
if args.num == 1:
    p = vlc.MediaPlayer('1.mp3')
elif args.num == 2:
    p = vlc.MediaPlayer('2.mp3')
elif args.num == 3:
    p = vlc.MediaPlayer('3.mp3')
    pass

p.play()
time.sleep(0.5)
duration = p.get_length() / 1000
print(duration)
time.sleep(duration)
