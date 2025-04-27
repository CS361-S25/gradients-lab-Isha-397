#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"

emp::web::Document doc{"target"};

/**
 * @class GradientAnimator
 * @brief Animates a grid of grayscale cells that shift horizontally with fading over time.
 *
 * Inherits from emp::web::Animate to enable interactive animations using Empirical.
 * Each cell has a float value [0.0, 1.0] mapped to grayscale via HSV coloring.
 */
class GradientAnimator : public emp::web::Animate {
private:
    int num_w_boxes;  ///< Number of boxes horizontally
    int num_h_boxes;  ///< Number of boxes vertically
    int box_size;     ///< Pixel size of each box
    emp::web::Canvas canvas;  ///< Canvas for drawing the grid
    std::vector<std::vector<float>> cells;      ///< Current state of cells (float values)
    std::vector<std::vector<float>> next_cells; ///< Next step's cell states

public:
    /**
     * @brief Constructor for GradientAnimator.
     * Initializes grid size, canvas, button controls, and starting cell values.
     */
    GradientAnimator()
        : num_w_boxes(50),
          num_h_boxes(50),
          box_size(10),
          canvas(num_w_boxes * box_size, num_h_boxes * box_size)
    {
        // Add canvas and control buttons to the document
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");

        // Initialize cells with default value 0.0
        cells.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0.0f));
        next_cells.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0.0f));

        // Set a few cells to higher initial values
        cells[5][5] = 1.0f;
        cells[10][10] = 0.8f;
        cells[15][15] = 0.6f;
    }

    /**
     * @brief Defines what happens at each animation frame.
     *
     * Clears the canvas, updates the grid by shifting values rightward
     * (with fading), and redraws the grid based on updated values.
     */
     void DoFrame() override {
        canvas.Clear();
    
        // First, compute the next state based on the current state
        for (int x = 0; x < num_w_boxes; ++x) {
            for (int y = 0; y < num_h_boxes; ++y) {
                // Wrap around horizontally
                if (x == 0) {
                    next_cells[x][y] = cells[num_w_boxes - 1][y];  // Wrap from right to left
                } else {
                    // Move the shade one cell to the right with slight fading
                    next_cells[x][y] = cells[x - 1][y] * 0.99f;
                }
            }
        }
    
        // Swap the buffers: next becomes current
        std::swap(cells, next_cells);
    
        // Clear next_cells for the next update
        for (auto &row : next_cells) {
            std::fill(row.begin(), row.end(), 0.0f);
        }
    
        // Draw the grid based on updated cell values
        for (int x = 0; x < num_w_boxes; ++x) {
            for (int y = 0; y < num_h_boxes; ++y) {
                float val = cells[x][y];
                canvas.Rect(
                    x * box_size,
                    y * box_size,
                    box_size,
                    box_size,
                    emp::ColorHSV(0, 0, val), //greyscale
                    "black" // Border color
                );
            }
        }
    }
};

// Global instance of animator
GradientAnimator animator;

/**
 * @brief Main function that starts the animation.
 */
int main() {
    animator.Step();
}
