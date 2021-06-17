import vlc
import time
p = vlc.MediaPlayer('3.mp3')
p.play()
time.sleep(0.5)
duration = p.get_length() / 1000
print(duration)
time.sleep(duration)
