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


function [div] = divergence(w, dx)
% nabla(dot)w
    div = (w(x+1,y)-w(x-1,y) + w(x,y+1)-w(x,y-1))*dx;
end

function [uNew] = gradiantSubtraction(w, p, dx)
%  du/dt = u(dot)nabla*u - (1/rho)*grad(p) + F
% OBS dx = 0.5 => 1/2
% equation 8
    uNew = w;
    uNew = uNew - (p(x+1,y) - p(x-1,y)) - (p(x,y+1) - p(x,y-1))*dx;
end




A(1,:) = discreteLap;
discreteLap = [0 1 0; 
               1 -4 1; 
               0 1 0];