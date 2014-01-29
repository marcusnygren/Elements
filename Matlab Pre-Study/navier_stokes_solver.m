function [ ] = main()
    
    nrFrames = 10;
    nrParticles = 10;
    timeStep = 0.1;
    gridSizeX = 100;
    gridSizeY = 100;

    velocityTexture = zeros(gridSizeX, gridSizeY, 2);
    preasureTexture = zeros(gridSizeX, gridSizeY);
    forces = zeros(gridSizeX, gridSizeY);
    forces((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles)) = 3;
    particles = zeros(gridSizeX, gridSizeY);
    particles((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles)) = 1;
    
    for frames = 1 : nrFrames
        newVelocityTexture = zeros(gridSizeX, gridSizeY, 100,2);
        newPreasureTexture = zeros(gridSizeX, gridSizeY);
        
        for x = 1 : gridSizeX;
            for y = 1 : gridSizeY;
                % Advect velocity.
                newVelocityTexture(x, y) = calculateAdvection(x, y, timeStep, 1 / gridSizeX, velocityTexture, velocityTexture);
                
                % Calculate Viscous Diffusion.
                
                % Add forces.
                newVelocityTexture(x, y) = addForces(x, y, forces, newVelocityTexture);
                
                % Compute Pressure.
                newPreasureTexture = calculateJacobiInteration(x, y, -(1/gridSizeX)^2, 4, preasureTexture, )
                % Substract pressure gradient.
            end
        end
        
        velocityTexture = newVelocityTexture;
        preasureTexture = newPreasureTexture;
    end
end

function [result] = calculateJacobiInteration(x, y, alpha, beta, texture1, texture2)

    result = (texture1(x-1,y) + texture1(x+1,y) + texture1(x,y-1) + texture1(x-1,y+1) + alpha*texture2(x,y)) / beta;

end

function [result] = calculateAdvection(x, y, timestep, dx, velocityTexture, advectionTexture)
    positionX = round(x - timestep * dx * velocityTexture(x,y,1));
    positionY = round(y - timestep * dx * velocityTexture(x,y,2));
    
    result = advectionTexture(positionX, positionY);
end

function [result] = addForces(x, y, forces, texture)
    result = texture(x, y) + forces(x, y); 
end











