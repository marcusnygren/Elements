function [F] = navier_stokes_solver()
    clc
    nrFrames = 100;
    nrParticles = 10;
    timeStep = 0.3;
    gridSizeX = 100;
    gridSizeY = 100;
    temp = 100*ones(gridSizeX, gridSizeY);
    dimension = 2;
    velocityTexture = zeros(gridSizeX, gridSizeY, dimension);
%     velocityTexture = 600*rand(gridSizeX, gridSizeY, dimension);
    preasureTexture = zeros(gridSizeX, gridSizeY, dimension);
    
%   constant force everywhere
      forces = zeros(gridSizeX, gridSizeY, dimension);
      forces(:,:,1) = 10;
      forces(:,:,2) = 10;
%     forces = ones(gridSizeX, gridSizeY, dimension);

%     forces((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles),1) = 1;
%     forces((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles),2) = 500;


    particles = zeros(gridSizeX, gridSizeY);
    particles((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles)) = 1;
    
    
%     fig = figure;
    
    for frames = 1 : nrFrames
        newVelocityTexture = zeros(gridSizeX, gridSizeY, dimension);
        newPreasureTexture = zeros(gridSizeX, gridSizeY, dimension);
        divergenceTexture = zeros(gridSizeX, gridSizeY);
        
        for x = 2 : (gridSizeX - 1);
            for y = 2 : (gridSizeY - 1);
                
                % Advect velocity.
                newVelocityTexture(x, y) = calculateAdvection(x, y, timeStep, 1 / gridSizeX, velocityTexture, velocityTexture);
                
                % Advect particles
                particles(x, y) = calculateAdvection(x, y, timeStep, 1 / gridSizeX, velocityTexture, particles);
                % TODO: TRACE BACKWARDS. The particles can only 'move' if
                % particles(x,y) =/= 0. Therefore, check why
                % particles(x,y) = 0 for all x and y.
                % A possible scenario is: particles(x,y) = 1 only when
                % 40 < x < 60 and 40 < y < 60. Check this too.
                
                

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
        
        % Hörn
            newVelocityTexture(2,2,:) = calculateBoundary(1,1,velocityTexture,1,1,-1);
            newVelocityTexture(2,gridSizeY-1,:) = calculateBoundary(1,gridSizeY,velocityTexture,1,-1,-1);
            newVelocityTexture(gridSizeX-1,2,:) = calculateBoundary(gridSizeX,1,velocityTexture,-1,1,-1);
            newVelocityTexture(gridSizeX-1,gridSizeY-1,:) = calculateBoundary(gridSizeX,gridSizeY,velocityTexture,-1,-1,-1);
            
            
        % Vänster
            for x = 2 : gridSizeX-1;
                newVelocityTexture(x,2,:) = calculateBoundary(x,1,velocityTexture,0,1,-1);
            end
        % höger
            for x = 2 : gridSizeX-1;
                
                newVelocityTexture(x,gridSizeY-1,:) = calculateBoundary(x,gridSizeY,velocityTexture,0,-1,-1);
            end
        
        % Upp      
            for y = 2 : gridSizeY-1;
                 newVelocityTexture(2,y,:) = calculateBoundary(1,y,velocityTexture,1,0,-1);
            end
        % Ner
            for y = 2 : gridSizeY-1;
                 newVelocityTexture(gridSizeX-1,y,:) = calculateBoundary(gridSizeX,y,velocityTexture,-1,0,-1);
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
       
end

% Jacobi iteration, pressure difference. texture1 = pressure, texture2 = divergence

% Check if this function is correct.
function [result] = calculateJacobiInteration(x, y, alpha, beta, texture1, texture2)

    result = (texture1(x-1,y,1) + texture1(x+1,y,1) + texture1(x,y-1,2) + texture1(x-1,y+1,2) + alpha*texture2(x,y)) / beta;
    
end

% Advection, Euler implicit

% TODO: Implement interpolation (typ bilinear). Do it before round() or
% after?
function [result] = calculateAdvection(x, y, timestep, dx, velocityTexture, advectionTexture)
% timestep * dx * velocityTexture(x,y,1)
% timestep * dx * velocityTexture(x,y,1)
    
    X = x - timestep * dx * velocityTexture(x,y,1);
    Y = y - timestep * dx * velocityTexture(x,y,2);

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

function [result] = calculateBoundary(x, y, texture, offsetX, offsetY, scale)
    result = scale*texture(x + offsetX, y + offsetY);
end