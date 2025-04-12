#ifndef MANTRI_H
#define MANTRI_H
/**
 * @file mantri.h 
 * @brief Defines the heatmap
 * 
 * This file conatins all the description required to generate the proper heatmap
 */
#include <QGraphicsScene>

class MantriGraphics;

/**
 * @class Mantri
 * @brief Defines the heatmaps and their graphical aspects
 * 
 * It defines the various functions for generating the heatmap
 */
class Mantri{
public:
    /**
     * Initializes the constructor
     */
    Mantri(){};
    /**
     * @brief The various parameters are declared
     * @param *scene Refers to the value of the pointer scene
     * @param &&point Refers to the adress of the adress of the point (position of the bot)
     * @param intensity Refers to the intensity of the color
     * @param width Refers to the width of the window
     * @param radius Refers to the length and width of the heatmap's nbounding rectangle
     */
    Mantri(QGraphicsScene *scene, QPointF &&point, float intensity, float width, float radius);
    /**
     * @brief Function to update the color
     * @param intensity Takes the intensity of the color
     * @param paint To actually color the larger structure on the basis of the intensity
     */
    void updateColor(float intensity, bool paint);
    /**
     * @brief Function to get the x position 
     * @return Returns the x position
     */
    float getx(){return x;}
    /**
     * @brief Function to get the y position 
     * @return Returns the y position
     */
    float gety(){return y;}
private:
    /**
     *The various paramters have their typedefs declared
     */
    float x, y, radius, width, intensity;
    QGraphicsRectItem *graphics = nullptr;
};

#endif // MANTRI_H
