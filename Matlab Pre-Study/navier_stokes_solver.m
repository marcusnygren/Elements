function [ ] = main()
    
    nrFrames = 100;
    gridSizeX = 100;
    gridSizeY = 100;

    velocityTexture = zeros(gridSizeX, gridSizeY, 100,2);
    preasureTexture = zeros(gridSizeX, gridSizeY);
    
    for frames = 1 : nrFrames
        for x = 1 : gridSizeX;
            for y = 1 : gridSizeY;

            end
        end
    end
end

function [result] = calculateJacobiInteration(x, y, alpha, beta, texture1, texture2)

    result = (texture1(x-1,y) + texture1(x+1,y) + texture1(x,y-1) + texture1(x-1,y+1) + alpha*texture2(x,y)) / beta;

end

function [] = calculateAdvection(x, y, timestep, dx, velocityTexture, advectionTexture)
    positionX = x - timestep * dx * velocityTexture(x,y,1);
    positionY = y - timestep * dx * velocityTexture(x,y,2);
    
    result = advectionTexture(positionX, positionY);
end