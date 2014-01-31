function [sameSame] = navier_stokes_solver()
    clc
    nrFrames = 100;
    nrParticles = 10;
    timeStep = 0.3;
    gridSizeX = 100;
    gridSizeY = 100;

    dimension = 2;
    velocityTexture = zeros(gridSizeX, gridSizeY, dimension);
%     velocityTexture = 600*rand(gridSizeX, gridSizeY, dimension);
    preasureTexture = zeros(gridSizeX, gridSizeY, dimension);
    
%   constant force everywhere
    forces = zeros(gridSizeX, gridSizeY, dimension);
%     forces = ones(gridSizeX, gridSizeY, dimension);

%     forces((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles),1) = 1;
    forces((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles),2) = 100;

%     Checking if the rendering is ok. It is. (put this somewhere inside
%     the y-for-loop and don't forget to remove 'particles(x,y) = calculateAd...'
%     particles(x,y) = 0;
%     r = randi([1 99],1,2);
%     particles(r(1),r(2)) = 1;



    particles = zeros(gridSizeX, gridSizeY);
    particles((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles)) = 1;
    
    fig = figure;
    
    
    for frames = 1 : nrFrames
        newVelocityTexture = zeros(gridSizeX, gridSizeY, dimension);
        newPreasureTexture = zeros(gridSizeX, gridSizeY, dimension);
        divergenceTexture = zeros(gridSizeX, gridSizeY);
        
        for x = 10 : (gridSizeX - 10);
            for y = 10 : (gridSizeY - 10);
                
                % Advect velocity.
                newVelocityTexture(x, y) = calculateAdvection(x, y, timeStep, 1 / gridSizeX, velocityTexture, velocityTexture);
                
                % Advect particles
                
                % TODO: TRACE BACKWARDS. The particles can only 'move' if
                % particles(x,y) =/= 0. Therefore, check why
                % particles(x,y) = 0 for all x and y.
                % A possible scenario is: particles(x,y) = 1 only when
                % 40 < x < 60 and 40 < y < 60. Check this too.
                
                particles(x, y) = calculateAdvection(x, y, timeStep, 1 / gridSizeX, velocityTexture, particles);

                % Calculate Viscous Diffusion.
                
                % Add forces.
                newVelocityTexture(x, y,:) = addForces(x, y, forces, newVelocityTexture);
                
                % Comupte divergence
                divergenceTexture(x, y) = divergence(x, y, velocityTexture, 1/gridSizeX);
                
                % Compute Pressure.
                for i = 1 : 40
                    newPreasureTexture(x, y) = calculateJacobiInteration(x, y, -(1/gridSizeX)^2, 4, newPreasureTexture, divergenceTexture);
                end
                % Substract pressure gradient.
%                 newVelocityTexture(x, y,:) = gradientSubtraction(x, y, velocityTexture, preasureTexture, 1/gridSizeX);
                
            end
        end
        velocityTexture = newVelocityTexture;
        preasureTexture = newPreasureTexture;
        
%         figure
%         colormap gray;
%         imagesc(particles)4
        imshow(particles);
        F(frames) = getframe;
% %         % draw new 'frame' (imshow)
%         truesize(fig,[1000 1000])
%        drawnow()

    end
    
    close all
    imshow(F(1).cdata)
    figure
    imshow(F(nrFrames).cdata)
    
    
end

% Jacobi iteration, pressure difference. texture1 = pressure, texture2 = divergence

% Check if this function is correct.
function [result] = calculateJacobiInteration(x, y, alpha, beta, texture1, texture2)

    result = (texture1(x-1,y,1) + texture1(x+1,y,1) + texture1(x,y-1,2) + texture1(x-1,y+1,2) + alpha*texture2(x,y)) / beta;
    
end

% Advection, Euler implicit

% TODO: Implement interpolation (typ bilinear). Also: Check if this function 
% is correct.
function [result] = calculateAdvection(x, y, timestep, dx, velocityTexture, advectionTexture)
% timestep * dx * velocityTexture(x,y,1)
% timestep * dx * velocityTexture(x,y,1)
    positionX = round(x - timestep * dx * velocityTexture(x,y,1));
    positionY = round(y - timestep * dx * velocityTexture(x,y,2));
%     disp('==========================================')
% x
% y
% positionX
% positionY
%     if x == positionX && y == positionY
%        disp('hej')
%     end
    result = advectionTexture(positionX, positionY);
end

% Adding force: u = u + f
function [result] = addForces(x, y, forces, texture)
    result = texture(x, y,:) + forces(x, y,:);
end

% nabla(dot)w
function [div] = divergence(x, y, w, dx)

    div = (w(x+1,y,1)-w(x-1,y,1) + w(x,y+1,2)-w(x,y-1,2))*0.5*dx;
    
end

% Equation 8: du/dt = u(dot)nabla*u - (1/rho)*grad(p) + F
function [result] = gradientSubtraction(x, y, w, p, dx)
    uNew(x, y, :) = w(x, y,:);
    centralDiff = [p(x+1,y) - p(x-1,y); p(x,y+1) - p(x,y-1)]*0.5*dx;
    Ux = uNew(x,y,1);
    Uy = uNew(x,y,2);
    U = [Ux; Uy];
    final = U - centralDiff;
    result = final;
end