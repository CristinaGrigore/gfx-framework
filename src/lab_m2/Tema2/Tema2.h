#pragma once

#include <string>
#include <vector> 
using namespace std;
#include "components/simple_scene.h"
#include "core/gpu/frame_buffer.h"
#include <algorithm> 

namespace m2
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;
        

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void OpenDialog();
       
      //  void create_box();

//        void median_cut(vector<P>& source, std::vector<P>& destination, std::uint_fast32_t numColors, const bool differ, std::size_t width);
        void OnFileSelected(const std::string &fileName);

        void OnFileSelected(const std::string& fileName, unsigned char* imageData);

        // Processing effects
        void GrayScale();
        void SaveImage(const std::string &fileName);
        vector<int> r, g, b;
        vector<vector<int>> all_pixels;
        bool reduce_colors;
        unsigned char* new_imageData;


     private:
        Texture2D *originalImage;
        Texture2D *processedImage, *transparentImage;

        int outputMode;
        bool gpuProcessing;
        bool saveScreenToImage;
        vector<vector<float>> bucket;
       
    };
}   // namespace m2
