#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"

emp::web::Document doc{"target"};

class GradientAnimator : public emp::web::Animate{
    private:
        int num_w_boxes;
        int num_h_boxes;
        int box_size;
        emp::web::Canvas canvas;
        // int number = 0;
        // double value = 0.5;
        std::vector<std::vector<float>> cells;
        std::vector<std::vector<float>> next_cells;
        // const int num_w_boxes = 40;
        // const int num_h_boxes = 40;
        // const int box_size = 10;

        

    public:
        GradientAnimator() 
            : num_w_boxes(50),  // <-- Initialize values here
            num_h_boxes(50),
            box_size(10),
            canvas(num_w_boxes * box_size, num_h_boxes * box_size) 
            {
        // canvas(400, 400) {
            doc << canvas;
            doc << GetToggleButton("Toggle");
            doc << GetStepButton("Step");

            cells.resize(num_w_boxes, std::vector<float>(num_h_boxes,0));
            next_cells.resize(num_w_boxes, std::vector<float>(num_h_boxes, 0));

            //few cells manually
            cells[5][5] = 1.0;
            cells[10][10] = 0.8;
            cells[15][15] = 0.6;
        }

        void DoFrame() override{
            canvas.Clear();
            for (int x = 0; x < num_w_boxes - 1; x++) {
                for (int y = 0; y < num_h_boxes; y++) {
                    // float val = cells[x][y];
                    // emp::Color color = emp::ColorHSV(0, 0, val);
                    // canvas.Rect(x * box_size, y * box_size, box_size, box_size, color, "black");
                    next_cells[x + 1][y] = cells[x][y] * 0.99; // Slight fade
                }
            }

            std::swap(cells, next_cells);

            for (auto &row : next_cells) {
                std::fill(row.begin(), row.end(), 0);
            }

            for (int x = 0; x < num_w_boxes; x++) {
                for (int y = 0; y < num_h_boxes; y++) {
                    float val = cells[x][y];
                    canvas.Rect(x * box_size, y * box_size, box_size, box_size,
                        emp::ColorHSV(0, 0, val), "black");
                }
            }
        }

};

GradientAnimator animator;

int main() {
    animator.Step();
}