function [particles] = navier_stokes_solver()
    clc
    nrFrames = 100;
    nrParticles = 10;
    timeStep = 5;
    gridSizeX = 100;
    gridSizeY = 100;

    dimension = 2;
    velocityTexture = zeros(gridSizeX, gridSizeY, dimension);
    preasureTexture = zeros(gridSizeX, gridSizeY);
    
    forces = zeros(gridSizeX, gridSizeY, dimension);
    
    forces((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles),1) = 1;
    forces((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles),2) = 3;

    particles = zeros(gridSizeX, gridSizeY);
    particles((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles)) = 1;
    
    for frames = 1 : nrFrames
        newVelocityTexture = zeros(gridSizeX, gridSizeY, dimension);
        newPreasureTexture = zeros(gridSizeX, gridSizeY);
        divergenceTexture = zeros(gridSizeX, gridSizeY);
        
        for x = 10 : (gridSizeX - 10);
            for y = 10 : (gridSizeY - 10);
                
                % Advect velocity.
                newVelocityTexture(x, y) = calculateAdvection(x, y, timeStep, 1 / gridSizeX, velocityTexture, newVelocityTexture);
                
                % Advect particles
                particles(x, y) = calculateAdvection(x, y, timeStep, 1 / gridSizeX, velocityTexture, particles);
%                 particles(x,y) = 0;
                
                % Calculate Viscous Diffusion.
                
                % Add forces.
                newVelocityTexture(x, y) = addForces(x, y, forces, newVelocityTexture);
                
                % Comupte divergence
                divergenceTexture(x, y) = divergence(x, y, velocityTexture, 1/gridSizeX);
                
                % Compute Pressure.
                newPreasureTexture(x, y) = calculateJacobiInteration(x, y, -(1/gridSizeX)^2, 4, preasureTexture, divergenceTexture);
                
                % Substract pressure gradient.
                newVelocityTexture(x, y,:) = gradientSubtraction(x, y, velocityTexture, preasureTexture, 1/gridSizeX);
                
            end
        end
        velocityTexture = newVelocityTexture;
        preasureTexture = newPreasureTexture;
        imshow(particles)
        drawnow()

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

function [div] = divergence(x, y, w, dx)
% nabla(dot)w
    div = (w(x+1,y,1)-w(x-1,y,1) + w(x,y+1,2)-w(x,y-1,2))*0.5*dx;
    
end

function [result] = gradientSubtraction(x, y, w, p, dx)
    %  du/dt = u(dot)nabla*u - (1/rho)*grad(p) + F
    % OBS dx = 0.5 => 1/2
    % equation 8

    uNew(x, y, :) = w(x, y,:);

    centralDiff = [p(x+1,y) - p(x-1,y); p(x,y+1) - p(x,y-1)]*0.5*dx;
    %     size(temp)
    %     uNew(x, y, :) = uNew(x, y, :) - ;
    Ux = uNew(x,y,1);
    Uy = uNew(x,y,2);
    U = [Ux; Uy];
    final = U - centralDiff;
    result = final;
end


