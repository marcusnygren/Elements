function [] = navier_stokes_solver()
    clc
    tic;
    nrParticles = 10;
    nrFrames = 200;
    timeStep = 0.5;
    gridSizeX = 50;
    gridSizeY = 50;
    dimension = 2; 
    velocityTexture = zeros(gridSizeX, gridSizeY, dimension);
%     velocityTexture = 600*rand(gridSizeX, gridSizeY, dimension);

%     Sätt INTE konstant tryck överallt. Det blir 0 vid subtraction, eller nåt - men för verklig röksimulering behöver
%     vi räkna ut ett nytt tryck vid varje frame
    preasureTexture = ones(gridSizeX, gridSizeY, dimension);
%     preasureTexture(:,:,1) = 300;
%     preasureTexture(:,:,2) = 300;
%     preasureTexture = 500*rand(gridSizeX, gridSizeY, dimension);
    
%   constant force everywhere
    forces = 500*rand(gridSizeX, gridSizeY, dimension);
%     forces = zeros(gridSizeX, gridSizeY, dimension);
%     forces(:,:,1) = 0;
%     forces(:,:,2) = 0;
      
%     forces((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles),1) = 50;
%     forces((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles),2) = 1;

    
    particles = zeros(gridSizeX, gridSizeY);
    particles((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles)) = 1;
    
%     fig = figure;
    
    for frames = 1 : nrFrames
        newVelocityTexture = zeros(gridSizeX, gridSizeY, dimension);
%         TODO: FIXA newPreasure
        newPreasureTexture = zeros(gridSizeX, gridSizeY, dimension);
        divergenceTexture = zeros(gridSizeX, gridSizeY);
        for x = 1 : gridSizeX;
            for y = 1 : gridSizeY;
%                 particles((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles)) = 1;

                % Advect velocity.
                newVelocityTexture(x, y,:) = calculateAdvection(x, y, timeStep, 1 / gridSizeX, velocityTexture, velocityTexture);
                
                % Advect particles
                
                particles(x, y) = calculateAdvection(x, y, timeStep, 1 / gridSizeX, velocityTexture, particles);
                
                % Add forces.
                newVelocityTexture(x, y,:) = addForces(x, y, forces, newVelocityTexture);
                % Comupte divergence
                divergenceTexture(x, y) = divergence(x, y, velocityTexture, 1/gridSizeX);
                
                % Compute Pressure.
                % TODO: FIXA LOOPEN SÅ ATT DEN INTE ÄR 0 HELA TIDEN
%                 for i = 1 : 20
%                     newPreasureTexture(x, y,:) = calculateJacobiInteration(x, y, -(1/gridSizeX)^2, 4, newPreasureTexture, divergenceTexture);
%                 end
%                 if newPreasureTexture(x,y,1) > 1 || newPreasureTexture(x,y,2) > 1
%                     newPreasureTexture(x,y,:)
%                 end
                % Substract pressure gradient.
                newVelocityTexture(x, y,:) = gradientSubtraction(x, y, velocityTexture, preasureTexture, 1/gridSizeX);
%                 nnz(newVelocityTexture(:))
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

            newVelocityTexture(x,gridSizeY,2) = -1*velocityTexture(x, gridSizeY - 1,2);
        end

    % Upp      
        for y = 2 : gridSizeY-1;
             newVelocityTexture(1,y,1) = -1*velocityTexture(1 + offsetX, y,1);
        end
    % Ner
        for y = 2 : gridSizeY-1;
             newVelocityTexture(gridSizeX,y,1) = -1*velocityTexture(gridSizeX - 1, y,1);
        end
            
        velocityTexture(:,:,:) = newVelocityTexture(:,:,:);
        % Om vi har en konstant pressure så behöver vi inte uppdatera till
        % en ny varje frame. Precis som forces() - MEN DEN SKA RÄKNAS FRAM
%         preasureTexture(:,:,:) = newPreasureTexture(:,:,:);
        
%         figure
%         colormap gray;
%         imagesc(newVelocityTexture(:,:,2));
        imshow(particles);
        F(frames) = getframe;
%        drawnow()
    end
end

% Jacobi iteration, pressure difference. texture1 = pressure, texture2 = divergence
% Check if this function is correct.
function [result] = calculateJacobiInteration(x, y, alpha, beta, texture1, texture2)
    
    grid = 50;
    if x == 1 && y == 1
%       Rätta till det här senare... till första ordningen eller andra
%       ordningen men ej laplacianen(?)
        result = (texture1(x+1,y,1) - texture1(x,y,1) + texture1(x,y+1,2) - texture1(x,y,2) +  alpha*texture2(x,y))/beta;
    elseif x == 1 && y == grid
        result = (texture1(x+1,y,1) - texture1(x,y,1) + texture1(x,y,2) - texture1(x,y-1,2) +  alpha*texture2(x,y))/beta;
    elseif x == grid && y == 1
        result = (texture1(x,y,1) - texture1(x-1,y,1) + texture1(x,y+1,2) - texture1(x,y,2) +  alpha*texture2(x,y))/beta;
    elseif x == grid && y == grid
        result = (texture1(x,y,1) - texture1(x-1,y,1) + texture1(x,y,2) - texture1(x,y-1,2) +  alpha*texture2(x,y))/beta;
    elseif x == 1
        result = (texture1(x+1,y,1) + texture1(x,y,1) + texture1(x,y-1,2) + texture1(x,y+1,2) + alpha*texture2(x,y)) / beta;
    elseif y == 1
        result = (texture1(x-1,y,1) + texture1(x+1,y,1) + texture1(x,y,2) + texture1(x,y+1,2) + alpha*texture2(x,y)) / beta;
    elseif x == grid
        result = (texture1(x-1,y,1) - texture1(x,y,1) + texture1(x,y-1,2) + texture1(x,y+1,2) + alpha*texture2(x,y)) / beta;
    elseif y == grid
        result = (texture1(x-1,y,1) + texture1(x+1,y,1) + texture1(x,y,2) - texture1(x,y-1,2) + alpha*texture2(x,y)) / beta;
    else
        result = (texture1(x-1,y,1) + texture1(x+1,y,1) + texture1(x,y-1,2) + texture1(x,y+1,2) + alpha*texture2(x,y)) / beta;
    end
    
%     result = 0;
end

% Advection, Euler implicit
function [result] = calculateAdvection(x, y, timestep, dx, velocityTexture, advectionTexture)
% timestep * dx * velocityTexture(x,y,1)
% timestep * dx * velocityTexture(x,y,1)
    
    X = x - timestep * dx * velocityTexture(x,y,1);
    Y = y - timestep * dx * velocityTexture(x,y,2);
    grid = 50;
    if x == 1 && y == 1
        result = advectionTexture(x+1,y+1);
    elseif x == 1 && y == grid
        result = -1*advectionTexture(x+1,y-1);
    elseif x == grid && y == 1
        result = -1*advectionTexture(x-1,y+1);
    elseif x == grid && y == grid
        result = -1*advectionTexture(x-1,y-1);
    elseif x == 1
%         -1*advectionTexture(x+1,y)
        result = -1*advectionTexture(x+1,y);
    elseif x == grid
%         -1*advectionTexture(x-1,y)
        result = -1*advectionTexture(x-1,y);
    elseif y == 1
%         -1*advectionTexture(x,y+1)
        result = -1*advectionTexture(x,y+1);
    elseif y == grid
%         -1*advectionTexture(x,y-1)
        result = -1*advectionTexture(x,y-1);
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
    grid = 50;
    if x == 1 && y == 1
        div = (w(x+1,y,1) - w(x,y,1)) + (w(x,y+1,2) - w(x,y,2))*dx;
    elseif x == grid && y == 1
        div = (w(x,y,1) - w(x-1,y,1)) + (w(x,y+1,2) - w(x,y,2))*dx;
    elseif x == 1 && y == grid
        div = (w(x+1,y,1) - w(x,y,1)) + (w(x,y,2) - w(x,y-1,2))*dx;
    elseif x == grid && y == grid
        div = (w(x,y,1) - w(x-1,y,1)) + (w(x,y,2) - w(x,y-1,2))*dx;
    elseif x == 1
        div = (w(x+1,y,1)-w(x,y,1) + (w(x,y+1,2) - w(x,y-1,2))/2)*dx;
    elseif y == 1
        div = ((w(x+1,y,1)-w(x-1,y,1))/2 + w(x,y+1,2) - w(x,y,2))*dx;
    elseif x == grid
        div = ((w(x,y,1)-w(x-1,y,1)) + (w(x,y+1,2) - w(x,y-1,2))/2)*dx;
    elseif y == grid
        div = ((w(x+1,y,1)-w(x-1,y,1))/2 + w(x,y,2) - w(x,y-1,2))*dx;
    else
        div = (w(x+1,y,1)-w(x-1,y,1) + w(x,y+1,2)-w(x,y-1,2))*0.5*dx;
    end
end

% Equation 8: du/dt = u(dot)nabla*u - (1/rho)*grad(p) + F
function [result] = gradientSubtraction(x, y, w, p, dx)
    grid = 50;
    if x == 1 && y == 1
        diff = [p(x+1,y) - p(x,y); p(x,y+1) - p(x,y)]*dx;
    elseif x == grid && y == 1
        diff = [p(x,y) - p(x-1,y); p(x,y+1) - p(x,y)]*dx;
    elseif x == 1 && y == grid
        diff = [p(x+1,y) - p(x,y); p(x,y+1) - p(x,y)]*dx;
    elseif x == grid && y == grid
        diff = [p(x,y) - p(x-1,y); p(x,y) - p(x,y-1)]*dx;
    elseif x == 1
        diff = [(p(x+1,y) - p(x,y)) ; (p(x,y+1) - p(x,y-1))*0.5]*dx;
    elseif y == 1
        diff = [(p(x+1,y) - p(x-1,y))*0.5 ; (p(x,y+1) - p(x,y))]*dx;
    elseif x == grid
        diff = [p(x,y) - p(x-1,y); (p(x,y+1) - p(x,y-1))*0.5]*dx;
    elseif y == grid
        diff = [(p(x+1,y) - p(x-1,y))*0.5; p(x,y) - p(x,y-1)]*dx;
    else
        diff = [p(x+1,y) - p(x-1,y); p(x,y+1) - p(x,y-1)]*0.5*dx;
    end
    uNew(x, y, :) = w(x, y,:);
    Ux = uNew(x,y,1);
    Uy = uNew(x,y,2);
    U = [Ux; Uy];
    final = U - diff;
    result = final;
end

% function [result] = calculateBoundary(x, y, texture, offsetX, offsetY, scale)
% %     texture(x + offsetX, y + offsetY,1)
% %     disp('=============================');
% %     texture(x + offsetX, y + offsetY,2)
%     result = scale*texture(x + offsetX, y + offsetY,:);
% end