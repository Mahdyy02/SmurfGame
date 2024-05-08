from PIL import Image, ImageDraw

def draw_grid(image_path, block_size):
    # Open the image
    img = Image.open(image_path)
    
    # Calculate image dimensions
    width, height = img.size
    
    # Create a draw object
    draw = ImageDraw.Draw(img)
    
    # Draw vertical grid lines
    for x in range(0, width, block_size):
        line = ((x, 0), (x, height))
        draw.line(line, fill=(255, 255, 255))  # White color
    
    # Draw horizontal grid lines
    for y in range(0, height, block_size):
        line = ((0, y), (width, y))
        draw.line(line, fill=(255, 255, 255))  # White color
    
    # Save the modified image
    img.save("grid_" + image_path)  # Save as a new image

# Path to your input image
input_image_path = "map.png"

# Define block size (e.g., 32x32 blocks)
block_size = 32

# Call the function to draw grid on the image
draw_grid(input_image_path, block_size)

print("Grid drawn successfully!")
