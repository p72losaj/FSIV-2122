/*!
  Esto es un esqueleto de programa para usar en las prácticas
  de Visión Artificial.

  Se supone que se utilizará OpenCV.

  Para compilar, puedes ejecutar:
    g++ -Wall -o esqueleto esqueleto.cc `pkg-config opencv --cflags --libs`

*/

#include <iostream>
#include <exception>
#include <sstream>

using namespace std;

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>

#include "common_code.hpp"

const cv::String keys =
    "{help h usage ? |      | Print this message}"
    "{r              | x,y,w,h | Use a rectangle (x,y, widht, height)}"
    "{c              | x,y,r | Use a circle (x,y,radius)}"
    "{p              | x1,y1,x2,y2,x3,y3 | Use a closed polygon x1,y1,x2,y2,x3,y3,...}"
    "{i              |     | Interactive mode.}"
    "{@input         | <none> | input image.}"
    "{@output        | <none> | output image.}"
    ;

void CallBackFunc(int event, int x, int y, int flags, void* userdata);

int
main (int argc, char* const* argv)
{
    int retCode=EXIT_SUCCESS;

    try {
        cv::CommandLineParser parser(argc, argv, keys);
        parser.about("Do a foreground enhance using a ROI.");
        if (parser.has("help"))
        {
            parser.printMessage();
            return EXIT_SUCCESS;
        }

        cv::String input_n = parser.get<cv::String>("@input");
        cv::String output_n = parser.get<cv::String>("@output");
        if (!parser.check())
        {
            parser.printErrors();
            return EXIT_FAILURE;
        }

        cv::Mat in = cv::imread(input_n, cv::IMREAD_UNCHANGED);
        if (in.empty())
        {
            std::cerr << "Error: could not open input image '" << input_n
                      << "'." << std::endl;
            return EXIT_FAILURE;
        }
        cv::Mat mask = in.clone();
        cv::Mat out = in.clone();

        //TODO

        cv::namedWindow("INPUT", cv::WINDOW_GUI_EXPANDED);
        cv::imshow("INPUT", in);
        cv::namedWindow("MASK",  cv::WINDOW_GUI_EXPANDED);
        cv::imshow("MASK", mask);

        // Obtenemos los puntos del rectangulo

       // cv::setMouseCallback("INPUT", CallBackFunc, NULL);

       out = convert_rgb_to_gray(in); // Transformamos la imagen de entrada

       out = convert_gray_to_rgb(out); // Obtenemos la imagen de salida

       // Modo por linea de comandos

       // Modo rectangulo

       if(parser.get<std::string>("r")!="x,y,w,h") //Así que el usuario a puesto -r=.... en la linea de comandos.
       {
           int x,y,width,height;
           char sep;
           std::istringstream buffer(parser.get<std::string>("r"));
           buffer>>x>>sep>>y>>sep>>width>>sep>>height;

           if(!buffer)
           {

               std::cerr << "Error:option'-r'incorrect."<<std::endl;
               return EXIT_FAILURE;
           }
           mask=generate_rectagle_mask(in.cols,in.rows,x,y,width,height,in.type()); // genera la mascara del rectangulo
           // mask = generate_rectagle_mask(in.cols,in.rows,100,50,200,200,in.type());
       }

       // Modo circulo

       else if(parser.get<std::string>("c")!="x,y,r") //Así que el usuario a puesto -c=.... en la linea de comandos.
       {
           int x,y,radius;
           char sep;
           std::istringstream buffer(parser.get<std::string>("c"));
           std::cerr << "Estoy en circulo y buffer: " << parser.get<std::string>("c") <<std::endl;
           buffer>>x>>sep>>y>>sep>>radius;

           if(!buffer)
           {

               std::cerr << "Error:option'-c'incorrect."<<std::endl;
               return EXIT_FAILURE;
           }
           mask=generate_circle_mask(in.cols,in.rows,x,y,radius,in.type()); // genera la mascara del rectangulo

       }

       // Modo poligono cerrado

       else if(parser.get<std::string>("p")!="x1,y1,x2,y2,x3,y3") //Así que el usuario a puesto -p=.... en la linea de comandos.
       {
           int x,y;
           char sep;
           std::istringstream buffer(parser.get<std::string>("p"));

           if(!buffer)
           {
               std::cerr << "Error:option'-p'incorrect."<<std::endl;
               return EXIT_FAILURE;
           }

           std::vector<cv::Point> puntos;

           do{
                buffer >> x >> sep >> y >> sep;
                cv::Point punto(x,y);
                puntos.push_back(punto);
           }while(buffer);

           if(puntos.size() > 2){
               mask=generate_polygon_mask(in.cols,in.rows,puntos,in.type());
           }

           else{
               std::cerr << "No es poligono" << std::endl;
           }

       }

       // Combinamos las imagenes

        out = combine_images(in,out,mask);

        //


        cv::namedWindow("OUTPUT",  cv::WINDOW_GUI_EXPANDED);
        cv::imshow("OUTPUT", out);

        int k = cv::waitKey(0)&0xff;
        if (k!=27)
            cv::imwrite(output_n, out);
    }
    catch (std::exception& e)
    {
        std::cerr << "Capturada excepcion: " << e.what() << std::endl;
        retCode = EXIT_FAILURE;
    }
    return retCode;
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{

     if  ( event == cv::EVENT_LBUTTONDOWN )
     {
          std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
     }
     else if  ( event == cv::EVENT_RBUTTONDOWN )
     {
          std::cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
     }
     else if  ( event == cv::EVENT_MBUTTONDOWN )
     {
          std::cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
     }

}
