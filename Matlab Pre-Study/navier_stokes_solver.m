function [F] = navier_stokes_solver()
    clc
    nrFrames = 100;
    nrParticles = 10;
    timeStep = 0.05;
    gridSizeX = 100;
    gridSizeY = 100;
    dimension = 2;
    velocityTexture = zeros(gridSizeX, gridSizeY, dimension);
%     velocityTexture = 600*rand(gridSizeX, gridSizeY, dimension);
    preasureTexture = zeros(gridSizeX, gridSizeY, dimension);
    
%   constant force everywhere
      forces = zeros(gridSizeX, gridSizeY, dimension);
      forces(:,:,1) = 20;
      forces(:,:,2) = 20;
      
%     forces((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles),1) = 1;
%     forces((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles),2) = 500;

    
    particles = zeros(gridSizeX, gridSizeY);
    particles((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles)) = 1;
    
%     fig = figure;
    
    for frames = 1 : nrFrames
        newVelocityTexture = zeros(gridSizeX, gridSizeY, dimension);
        newPreasureTexture = zeros(gridSizeX, gridSizeY, dimension);
        divergenceTexture = zeros(gridSizeX, gridSizeY);
        
        for x = 1 : gridSizeX;
            for y = 1 : gridSizeY;
                
                % Advect velocity.
                newVelocityTexture(x, y,:) = calculateAdvection(x, y, timeStep, 1 / gridSizeX, velocityTexture, velocityTexture);
                
                % Advect particles
                
                % Fade har ingenting med tiden att göra. 
                particles(x, y) = calculateAdvection(x, y, timeStep, 1 / gridSizeX, velocityTexture, particles);

                
                % Add forces.
                newVelocityTexture(x, y,:) = addForces(x, y, forces, newVelocityTexture);
                
                % Comupte divergence
%                 divergenceTexture(x, y) = divergence(x, y, velocityTexture, 1/gridSizeX);
                
                % Compute Pressure.
%                 for i = 1 : 40
%                     newPreasureTexture(x, y) = calculateJacobiInteration(x, y, -(1/gridSizeX)^2, 4, newPreasureTexture, divergenceTexture);
%                 end
                % Substract pressure gradient.
%                 newVelocityTexture(x, y,:) = gradientSubtraction(x, y, velocityTexture, preasureTexture, 1/gridSizeX);
                
            end
        end
        
        offsetX = 1;
        offsetY = 1;
        % Hörn
            newVelocityTexture(1,1,:) = -1*velocityTexture(1 + offsetX, 1 + offsetY,:);
            newVelocityTexture(1,gridSizeY,:) = -1*velocityTexture(1 + offsetX,gridSizeY - offsetY,:);
            newVelocityTexture(gridSizeX,1,:) = -1*velocityTexture(gridSizeX - offsetX,1 + offsetY,:);
            newVelocityTexture(gridSizeX,gridSizeY,:) =  -1*velocityTexture(gridSizeX - offsetX, gridSizeY - offsetY,:);
            
            
        % Vänster
            for x = 2 : gridSizeX-1;
                newVelocityTexture(x,1,2) = -1*velocityTexture(x, 1 + offsetY,2);
            end
        % höger
            for x = 2 : gridSizeX-1;
                
                newVelocityTexture(x,gridSizeY,:) = -1*velocityTexture(x, gridSizeY - 1,2);
            end
        
        % Upp      
            for y = 2 : gridSizeY-1;
                 newVelocityTexture(1,y,:) = -1*velocityTexture(1 + offsetX, y,1);
            end
        % Ner
            for y = 2 : gridSizeY-1;
                 newVelocityTexture(gridSizeX,y,:) = -1*velocityTexture(gridSizeX - 1, y,1);
            end
            
        
        velocityTexture(:,:,:) = newVelocityTexture(:,:,:);
        preasureTexture(:,:,:) = newPreasureTexture(:,:,:);

        
%         figure
%         colormap gray;
%         imagesc(newVelocityTexture(:,:,2));
    
        imshow(particles);
        F(frames) = getframe;
% %         % draw new 'frame' (imshow)
%         truesize(fig,[1000 1000])
%        drawnow()

    end
       
end

% Jacobi iteration, pressure difference. texture1 = pressure, texture2 = divergence

% Check if this function is correct.
function [result] = calculateJacobiInteration(x, y, alpha, beta, texture1, texture2)

%     result = (texture1(x-1,y,1) + texture1(x+1,y,1) + texture1(x,y-1,2) + texture1(x-1,y+1,2) + alpha*texture2(x,y)) / beta;
result = 0;
    
end

% Advection, Euler implicit

% TODO: Implement interpolation (typ bilinear). Do it before round() or
% after?
function [result] = calculateAdvection(x, y, timestep, dx, velocityTexture, advectionTexture)
% timestep * dx * velocityTexture(x,y,1)
% timestep * dx * velocityTexture(x,y,1)
    
    X = x - timestep * dx * velocityTexture(x,y,1);
    Y = y - timestep * dx * velocityTexture(x,y,2);
    if x == 1 && y == 1
        result = 0;
    elseif x == 1 && y == 100
        result = 0;
    elseif x == 100 && y == 1
        result = 0;
    elseif x == 100 && y == 100
        result = 0;
    elseif x == 1
        result = 0;
    elseif x == 100
        result = 0;
    elseif y == 1
        result = advectionTexture(x,y+1);
    elseif y == 100
        result = advectionTexture(x,y-1);
    else
        
    x1 = floor(X);
    x2 = ceil(X);
    y1 = floor(Y);
    y2 = ceil(Y);
    if(rem(X,1) == 0 && rem(Y,1) == 0)
        result = advectionTexture(X,Y);
    elseif (rem(X,1) == 0)
        result = ((y2-Y)/(y2-y1))*advectionTexture(X,y1) + ((Y-y1)/(y2-y1))*advectionTexture(X,y2);
    
    elseif (rem(Y,1) == 0)
        result = ((x2-X)/(x2-x1))*advectionTexture(x1,Y) + ((X-x1)/(x2-x1))*advectionTexture(x2,Y);
    else
        R1 = ((x2-X)/(x2-x1))*advectionTexture(x1,y1) + ((X-x1)/(x2-x1))*advectionTexture(x2,y1);
        R2 = ((x2-X)/(x2-x1))*advectionTexture(x1,y2) + ((X-x1)/(x2-x1))*advectionTexture(x2,y2);
        result = ((y2-Y)/(y2-y1))*R1 + ((Y-y1)/(y2-y1))*R2;
    end
    end
end

% Adding force: u = u + f
function [result] = addForces(x, y, forces, texture)
    result = texture(x, y,:) + forces(x, y,:);
end

% nabla(dot)w
function [div] = divergence(x, y, w, dx)
    
%     div = (w(x+1,y,1)-w(x-1,y,1) + w(x,y+1,2)-w(x,y-1,2))*0.5*dx;
    div = 0;
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

% function [result] = calculateBoundary(x, y, texture, offsetX, offsetY, scale)
% %     texture(x + offsetX, y + offsetY,1)
% %     disp('=============================');
% %     texture(x + offsetX, y + offsetY,2)
%     result = scale*texture(x + offsetX, y + offsetY,:);
% end