#include "lab_m2/Tema2/Tema2.h"

#include <vector>
#include <iostream>
#include <algorithm> 
#include "pfd/portable-file-dialogs.h"

using namespace std;
using namespace m2;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */
unsigned int width, height, channels;
struct P
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a = 0;
};

uint8_t redRange, greenRange, blueRange, alphaRange;
vector<P> box, source, destination;
bool operator==(const P& lhs, const P& rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
}

bool red_comp(const P& a, const P& b)
{
    return a.r < b.r;
}

bool green_comp(const P& a, const P& b)
{
    return a.g < b.g;
}

bool blue_comp(const P& a, const P& b)
{
    return a.b < b.b;
}

bool alpha_comp(const P& a, const P& b)
{
    return a.a < b.a;
}
void create_box(bool gpuProcessing, Texture2D *originalImage, Texture2D *processedImage)
{
    auto textureImage = (gpuProcessing == true) ? originalImage : processedImage;
    unsigned char* data = textureImage->GetImageData();
    P *pixel = new P;
    for (int i = 0; i < height * width * 3; i += 3)
    {
        pixel->r = data[i];
        pixel->g = data[i+1];
        pixel->b = data[i+2];

        box.push_back(*pixel);
    }

    //pastrez o versiune nemodificata a pixelilor in source
    source = box;
}
void sort_box(vector<P>& b)
{
    //determin culoarea dominanta i.e. culoarea cuprinsã în cel mai mare interval 
    auto r_max = (*max_element(b.begin(), b.end(), red_comp)).r;
    auto r_min = (*min_element(b.begin(), b.end(), red_comp)).r;
    auto g_max = (*max_element(b.begin(), b.end(), green_comp)).g;
    auto g_min = (*min_element(b.begin(), b.end(), green_comp)).g;
    auto b_max = (*max_element(b.begin(), b.end(), blue_comp)).b;
    auto b_min = (*min_element(b.begin(), b.end(), blue_comp)).b;
    auto a_max = (*max_element(b.begin(), b.end(), blue_comp)).a;
    auto a_min = (*min_element(b.begin(), b.end(), alpha_comp)).a;
    redRange = r_max - r_min;
    greenRange = g_max - g_min;
    blueRange = b_max - b_min;
    alphaRange = a_max - a_min;

    if (redRange >= greenRange && redRange >= blueRange && redRange >= alphaRange)
    {
        sort(b.begin(), b.end(), red_comp);
    }
    else if (greenRange >= redRange && greenRange >= blueRange && greenRange >= alphaRange)
    {
        sort(b.begin(), b.end(), green_comp);
    }
    else if (blueRange >= redRange && blueRange >= greenRange && blueRange >= alphaRange)
    {
        sort(b.begin(), b.end(), blue_comp);
    }
  
}
vector<P> get_colors(const int numColors)
{
    typedef vector<P> Box;
    //primul atribut semnifica intervalul (max - min) al culorii dominante in box-ul respectiv 
    typedef pair<uint8_t, Box> RangeBox;

    //continui sa generez boxes pana cand am atatea boxes cate culori vreau
    vector<RangeBox> boxes;
    Box init = source;
    boxes.push_back(RangeBox(0, init));

    while (boxes.size() < numColors)
    {
        // sortez fiecare box dupa culoarea dominanta 
        for (RangeBox& boxData : boxes)
        {
        
            if (get<0>(boxData) == 0)
            {
                sort_box(get<1>(boxData));
                if (redRange >= greenRange && redRange >= blueRange && redRange >= alphaRange)
                {
                    get<0>(boxData) = redRange;
                }
                else if (greenRange >= redRange && greenRange >= blueRange && greenRange >= alphaRange)
                {
                    get<0>(boxData) = greenRange;
                }
                else if (blueRange >= redRange && blueRange >= greenRange && blueRange >= alphaRange)
                {
                    get<0>(boxData) = blueRange;
                }
                else
                {
                    get<0>(boxData) = alphaRange;
                }
            }
        }

        //sortez in functie de range-ul care acopera cea mai mare parte din vectorul de pixeli
        sort(boxes.begin(), boxes.end(), [](const RangeBox& a, const RangeBox& b)
            {
                return get<0>(a) < get<0>(b);
            });

        // elimin box-ul cu range-ul maxim de culoare, pentru a-l imparti pe mijloc si re-adauga
        vector<RangeBox>::iterator itr = prev(boxes.end());
        Box biggestBox = get<1>(*itr);
        boxes.erase(itr);

        // split biggest box si il re-adaug in boxes, algoritmul continua
        Box splitA(biggestBox.begin(), biggestBox.begin() + biggestBox.size() / 2);
        Box splitB(biggestBox.begin() + biggestBox.size() / 2, biggestBox.end());

        boxes.push_back(RangeBox(0, splitA));
        boxes.push_back(RangeBox(0, splitB));
    }

    // calculez valoarea medie pentru fiecare interval
    vector<P> palette;
    for (const RangeBox& boxData : boxes)
    {
        Box box = get<1>(boxData);
        int red = 0;
        int green = 0;
        int blue = 0;
        int alpha = 0;
        for_each(box.begin(), box.end(), [&](const P& p)
            {
                red += p.r;
                green += p.g;
                blue += p.b;
                alpha += p.a;
            });
        int size = box.size();
        red /= size;
        green /= size;
        blue /= size;
        alpha /= size;

        // ma asigur ca valoarea medie nu depaseste 255
        palette.push_back(
            {
                static_cast<uint8_t>(min(blue, 255u)),
                static_cast<uint8_t>(min(green, 255u)),
                static_cast<uint8_t>(min(red, 255u)),
                static_cast<uint8_t>(min(alpha, 255u))
            });
    }
    return palette;
}

void median_cut(int numColors)
{
    //obtin noua paleta de culori
    vector<P> palette = get_colors(numColors);

    destination = source;

    for (int i = 0; i < destination.size(); i++)
    {
        P& p = destination.at(i);
        p = *min_element(palette.begin(), palette.end(), [p](const P& a, const P& b)
            {
                //calculez distanta euclidiana dintre pizeli ca sa determin ce pixel din paleta aprtoximeaza 
                //cel mai bine valoarea pixelului curent/din imaginea initiala
                auto _a = sqrt(pow(p.r - a.r, 2) + pow(p.g - a.g, 2) + pow(p.b - a.b, 2) + pow(p.a - a.a, 2));
                auto _b = sqrt(pow(p.r - b.r, 2) + pow(p.g - b.g, 2) + pow(p.b - b.b, 2) + pow(p.a - b.a, 2));
                return _a < _b;
            });
        destination.at(i) = p;
            
    }
    
  
}

Tema2::Tema2()
{
    outputMode = 0;
    gpuProcessing = false;
    saveScreenToImage = false;
    window->SetSize(600, 600);
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    // Load default texture fore imagine processing
    originalImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube", "lenna.png"), nullptr, "image", true, true);
    processedImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube", "lenna.png"), nullptr, "newImage", true, true);
    transparentImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube", "lenna.png"), nullptr, "redImage", true, true);
    printf("channels %d\n", transparentImage->GetNrChannels());
    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh2 = new Mesh("quad");
        mesh2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh2->UseMaterials(false);
        meshes[mesh2->GetMeshID()] = mesh2;
    }

    string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema2", "shaders");

    // Create a shader program for particle system
    {
        Shader* shader = new Shader("ImageProcessing");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    // Create a shader program for particle system
    {
        Shader* shader2 = new Shader("TranspImageProcessing");
        shader2->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader2->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);

        shader2->CreateAndLink();
        shaders[shader2->GetName()] = shader2;
    }
}


void Tema2::FrameStart()
{
}


void Tema2::Update(float deltaTimeSeconds)
{
    ClearScreen();

    auto shader = shaders["ImageProcessing"];
    shader->Use();

    if (saveScreenToImage)
    {
        window->SetSize(originalImage->GetWidth(), originalImage->GetHeight());
    }

    int flip_loc = shader->GetUniformLocation("flipVertical");
    glUniform1i(flip_loc, saveScreenToImage ? 0 : 1);

    int screenSize_loc = shader->GetUniformLocation("screenSize");
    glm::ivec2 resolution = window->GetResolution();
    glUniform2i(screenSize_loc, resolution.x, resolution.y);

    int outputMode_loc = shader->GetUniformLocation("outputMode");
    glUniform1i(outputMode_loc, outputMode);

    int locTexture = shader->GetUniformLocation("textureImage");
    glUniform1i(locTexture, 0);

    int locTexture1 = shader->GetUniformLocation("textureImage1");
    glUniform1i(locTexture1, 1);

    auto textureImage = (gpuProcessing == true) ? originalImage : processedImage;
    textureImage->BindToTextureUnit(GL_TEXTURE0);
    processedImage->BindToTextureUnit(GL_TEXTURE1);
    textureImage->GetSize(width, height);



    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
    
    
    RenderMesh(meshes["quad"], shader, glm::mat4(1));
    glDisable(GL_BLEND);
    
    if (saveScreenToImage)
    {
        saveScreenToImage = false;

        GLenum format = GL_RGB;
        if (originalImage->GetNrChannels() == 4)
        {
            format = GL_RGBA;
        }

        glReadPixels(0, 0, originalImage->GetWidth(), originalImage->GetHeight(), format, GL_UNSIGNED_BYTE, processedImage->GetImageData());
        processedImage->UploadNewData(processedImage->GetImageData());
        SaveImage("shader_processing_" + to_string(outputMode));

        float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
        window->SetSize(static_cast<int>(600 * aspectRatio), 600);
    }

}


void print_pixels(vector<vector<int>> all_pixels)
{
    for (int i = 0; i < all_pixels.size(); i++) {

        vector<int> p = all_pixels.at(i);
        printf("(%d, %d, %d) ", p.at(0), p.at(1), p.at(2));
    }
    cout << endl;

}

void Tema2::FrameEnd()
{
    DrawCoordinateSystem();
}


void Tema2::OnFileSelected(const string& fileName)
{
    if (fileName.size())
    {
        cout << fileName << endl;
        originalImage = TextureManager::LoadTexture(fileName, nullptr, "image", true, true);
        processedImage = TextureManager::LoadTexture(fileName, nullptr, "newImage", true, true);

        float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
        window->SetSize(static_cast<int>(600 * aspectRatio), 600);
    }
}


void Tema2::GrayScale()
{
    unsigned int channels = originalImage->GetNrChannels();
    unsigned char* data = originalImage->GetImageData();
    unsigned char* newData = processedImage->GetImageData();

    if (channels < 3)
        return;

    glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());

    for (int i = 0; i < imageSize.y; i++)
    {
        for (int j = 0; j < imageSize.x; j++)
        {
            int offset = channels * (i * imageSize.x + j);

            // Reset save image data
            char value = static_cast<char>(data[offset + 0] * 0.2f + data[offset + 1] * 0.71f + data[offset + 2] * 0.07);
            memset(&newData[offset], value, 3);
        }
    }

    processedImage->UploadNewData(newData);
}


void Tema2::SaveImage(const string& fileName)
{
    cout << "Saving image! ";
    processedImage->SaveToFile((fileName + ".png").c_str());
    cout << "[Done]" << endl;
}


void Tema2::OpenDialog()
{
    vector<string> filters =
    {
        "Image Files", "*.png *.jpg *.jpeg *.bmp",
        "All Files", "*"
    };

    auto selection = pfd::open_file("Select a file", ".", filters).result();
    if (!selection.empty())
    {
        cout << "User selected file " << selection[0] << "\n";
        OnFileSelected(selection[0]);
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F || key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE)
    {
        OpenDialog();
    }

    if (key == GLFW_KEY_E)
    {
        gpuProcessing = !gpuProcessing;
        if (gpuProcessing == false)
        {
            outputMode = 0;
        }
        cout << "Processing on GPU: " << (gpuProcessing ? "true" : "false") << endl;
       
    }
   
    if (key == GLFW_KEY_P)
    {
        create_box(gpuProcessing, originalImage, processedImage);
        median_cut(8);
        printf("\nsize %d %d %d\n", destination.size(), height, width);
        int i = 0;
        new_imageData = new unsigned char [height * width * 3];
        for (P p : destination)
        {
            new_imageData[i++] = p.r;
            new_imageData[i++] = p.g;
            new_imageData[i++] = p.b;
        }
        processedImage->UploadNewData(new_imageData);
    }
    if (key - GLFW_KEY_0 >= 0 && key < GLFW_KEY_5)
    {
        outputMode = key - GLFW_KEY_0;

        if (gpuProcessing == false)
        {
            outputMode = 0;
            GrayScale();
        }
    }

    if (key == GLFW_KEY_S && mods & GLFW_MOD_CONTROL)
    {
        if (!gpuProcessing)
        {
            SaveImage("processCPU_" + to_string(outputMode));
        }
        else {
            saveScreenToImage = true;
        }
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Tema2::OnWindowResize(int width, int height)
{
    // Treat window resize event
}