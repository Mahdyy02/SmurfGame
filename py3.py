from moviepy.editor import VideoFileClip
import os
from PIL import Image

def extract_frames(input_video, output_folder, start_min, end_min, frame_interval_ms):
    # Ensure the output folder exists
    os.makedirs(output_folder, exist_ok=True)
    
    # Convert minutes to seconds
    start_time = start_min * 60
    end_time = end_min * 60
    
    # Load the video file
    video = VideoFileClip(input_video)
    
    # Set the duration based on start and end time
    duration = end_time - start_time
    
    # Convert frame interval from milliseconds to seconds
    frame_interval_sec = frame_interval_ms / 1000.0
    
    # Iterate over the specified duration with the given frame interval
    t = start_time
    frame_count = 0
    while t < end_time:
        frame = video.get_frame(t)
        frame_image = Image.fromarray(frame)  # Convert NumPy array to PIL Image
        frame_path = os.path.join(output_folder, f"frame_{frame_count}.jpg")
        frame_image.save(frame_path)  # Save PIL Image to file
        
        # Increment time by frame interval
        t += frame_interval_sec
        frame_count += 1
    
    # Close the video file
    video.reader.close()

# Specify input video path, output folder, start minute, end minute, and frame interval in milliseconds
input_video_path = "vid.mp4"
output_folder_path = "output_frames"
start_minute = 3  # Minute to start extracting from
end_minute = 4    # Minute to end extracting at
frame_interval_ms = 80  # Frame interval in milliseconds (100 ms)

# Extract frames from specified video segment at the specified interval
extract_frames(input_video_path, output_folder_path, start_minute, end_minute, frame_interval_ms)
