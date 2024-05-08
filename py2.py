from PIL import Image
import numpy as np

def is_purple(pixel):
    """Check if the pixel color matches purple (RGB: 163, 73, 164)."""
    # Define the RGB values for the purple color
    purple_r, purple_g, purple_b = 163, 73, 164

    # Check if the pixel matches the purple color
    return (pixel[0] == purple_r and pixel[1] == purple_g and pixel[2] == purple_b)

def main():
    # Open the image
    image = Image.open("maaaaaaaaaaaap (1).png")

    # Get image dimensions
    width, height = image.size

    # Define block size and threshold
    block_size = 32
    purple_threshold = 0.2  # 70% of pixels should be purple

    # Convert the image to numpy array
    image_array = np.array(image)

    # Calculate the number of blocks in each dimension
    num_blocks_x = width // block_size
    num_blocks_y = height // block_size

    # Create an empty matrix to track purple blocks
    matrix = np.zeros((num_blocks_y, num_blocks_x), dtype=int)

    # Iterate through each block
    for i in range(num_blocks_y):
        for j in range(num_blocks_x):
            # Define the region of the image for the current block
            block_region = image_array[i*block_size:(i+1)*block_size, j*block_size:(j+1)*block_size]

            # Count purple pixels in the block
            purple_pixels_count = 0
            total_pixels = 0

            for pixel in block_region:
                for rgb in pixel:
                    if is_purple(rgb):
                        purple_pixels_count += 1
                    total_pixels += 1

            # Calculate the percentage of purple pixels in the block
            purple_percentage = purple_pixels_count / total_pixels

            # Check if the purple percentage meets the threshold
            if purple_percentage >= purple_threshold:
                matrix[i, j] = 1  # Set matrix element to 1 if block meets the threshold

    # Write the matrix to a file with comma-separated values
    np.savetxt('matrix.txt', matrix, fmt='%d', delimiter=',')

    print("Matrix written to 'matrix.txt'")

if __name__ == "__main__":
    main()
