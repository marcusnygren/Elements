function [c] = navier_stokes_solver()
    clc
    c = 0;    
    timeStep = 0.3;
    nrParticles = 10;
    nrFrames = 50;
    gridSizeX = 50;
    gridSizeY = 50;
    dimension = 2;
    h = 1/gridSizeX;
    velocityTexture = zeros(gridSizeX, gridSizeY, dimension);
    divergenceTexture = zeros(gridSizeX, gridSizeX);
    preasureTexture = zeros(gridSizeX, gridSizeY, dimension);
    T0 = 20;
    T = zeros(gridSizeX, gridSizeY);
    T((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles)) = 100;
    newT = zeros(gridSizeX, gridSizeY);
    rho = 0.5;
    rho_t = 1.3*ones(gridSizeX, gridSizeY);
    rho_t((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles)) = rho;
    newRho = zeros(gridSizeX,gridSizeY);
    % u1 - u0 / timeStep = a
    % F = m*a
%     preasureTexture(:, :, 1) = 100*rand(gridSizeX,gridSizeY);
%     preasureTexture(:, :, 2) = 130*rand(gridSizeX,gridSizeY);
    
%   constant force everywhere
    forces = -10*rand(gridSizeX, gridSizeY, dimension);
    forces(:,:,2) = 0;
    
    
    
%     forces((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles),1) = 50;
%     forces((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles),2) = 1;
    particles = zeros(gridSizeX, gridSizeY);
    particles((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles)) = 1;
    
%     fig = figure;
    
    for frames = 1 : nrFrames
        newVelocityTexture = zeros(gridSizeX, gridSizeY, dimension);
%         preasureTexture = zeros(gridSizeX, gridSizeY, dimension);
        newT = zeros(gridSizeX,gridSizeY);
        newRho = zeros(gridSizeX,gridSizeY);


       % För stabiltet: deltaT = h/u_max
       for x = 1 : gridSizeX;
            for y = 1 : gridSizeY;
                timeStep = h/(2*length(velocityTexture(x,y)));
%               particles((gridSizeX/2 - nrParticles) : (gridSizeX/2 + nrParticles), (gridSizeY/2 - nrParticles) : (gridSizeY/2 + nrParticles)) = 1;
                % Advect velocity.
                newVelocityTexture(x, y,:) = -calculateAdvection(x, y, timeStep, h, velocityTexture, velocityTexture);
                particles(x, y) = calculateAdvection(x, y, timeStep, h, velocityTexture, particles); 
            end
       end
       
       for x = 1 : gridSizeX-1;
            for y = 1 : gridSizeY-1;
%                 timeStep = h/(2*length(velocityTexture(x,y)));
                newT(x,y) = -timeStep*(velocityTexture(x,y,1)*(T(x+1,y)-T(x,y)) + velocityTexture(x,y,2)*(T(x,y+1)-T(x,y))) + T(x,y);
                newRho(x,y) = -timeStep*(velocityTexture(x,y,1)*(rho_t(x+1,y)-rho_t(x,y)) + velocityTexture(x,y,2)*(rho_t(x,y+1)-rho_t(x,y))) + rho_t(x,y);
            end
       end
       
       for x = 1 : gridSizeX;
            for y = 1 : gridSizeY;
                % Add forces.
                newVelocityTexture(x, y,:) = addForces(x, y, forces, newVelocityTexture, newT, T0,newRho);
            end
       end
       
       for x = 1 : gridSizeX;
           for y = 1 : gridSizeY;
                % Comupte divergence
                divergenceTexture(x, y) = divergence(x, y, velocityTexture, h);
           end
       end

%        ÄR preasureTexture(:,:,1) = preasureTexture(x, y,1); OCH
%        preasureTexture(:,:,2) = preasureTexture(x, y,2); RÄTT???????
       for k = 1 : 40 
            for x = 1 : gridSizeX;
                for y = 1 : gridSizeY;
                    % Compute Pressure
                    preasureTexture(x, y,:) = calculateJacobiIteration(x, y, -(h)^2, 4, preasureTexture, divergenceTexture);
                    preasureTexture(:,:,1) = preasureTexture(x, y,1);
                    preasureTexture(:,:,2) = preasureTexture(x, y,2);
                end
            end
       end

       for x = 1 : gridSizeX;
            for y = 1 : gridSizeY;
                % Substract pressure gradient.
                newVelocityTexture(x, y,:) = gradientSubtraction(x, y, newVelocityTexture, preasureTexture, h, rho);
            end
       end
        
                  
        % SKA DETTA VARA INNE I LOOPARNA? ANNARS RESETAS newVelocity till 0
%         offsetX = 1;
%         offsetY = 1;
%         % H?rn
%         newVelocityTexture(1,1,:) = -1*velocityTexture(1 + offsetX, 1 + offsetY,:);
%         newVelocityTexture(1,gridSizeY,:) = -1*velocityTexture(1 + offsetX,gridSizeY - offsetY,:);
%         newVelocityTexture(gridSizeX,1,:) = -1*velocityTexture(gridSizeX - offsetX,1 + offsetY,:);
%         newVelocityTexture(gridSizeX,gridSizeY,:) =  -1*velocityTexture(gridSizeX - offsetX, gridSizeY - offsetY,:);
% 
% 
%     % V?nster
%         for x = 2 : gridSizeX-1;
%             newVelocityTexture(x,1,2) = -1*velocityTexture(x, 1 + offsetY,2);
%         end
%     % h?ger
%         for x = 2 : gridSizeX-1;
%             newVelocityTexture(x,gridSizeY,2) = -1*velocityTexture(x, gridSizeY - 1,2);
%         end
% 
%     % Upp      
%         for y = 2 : gridSizeY-1;
%              newVelocityTexture(1,y,1) = -1*velocityTexture(1 + offsetX, y,1);
%         end
%     % Ner
%         for y = 2 : gridSizeY-1;
%              newVelocityTexture(gridSizeX,y,1) = -1*velocityTexture(gridSizeX - 1, y,1);
%         end

        velocityTexture(:,:,:) = newVelocityTexture(:,:,:);
        T(:,:) = newT(:,:);
        rho_t(:,:) = newRho(:,:);
%         figure
%         colormap gray;
%         imagesc(newVelocityTexture(:,:,2));
        imshow(particles);
        F(frames) = getframe;
%        drawnow()
    end
end

% Jacobi iteration, pressure difference. texture1 = pressure, texture2 = divergence
function [result] = calculateJacobiIteration(x, y, alpha, beta, texture1, texture2)
    
    grid = 50;
    if x == 1 && y == 1
%       R?tta till det h?r senare... till f?rsta ordningen eller n?t,
%       men ej laplacianen
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
end

% Advection, Euler implicit
function [result] = calculateAdvection(x, y, timestep, dx, velocityTexture, advectionTexture)    
    X = x - timestep * dx * velocityTexture(x,y,1);
    Y = y - timestep * dx * velocityTexture(x,y,2);
    grid = 50;
    if x == 1 && y == 1
        result = advectionTexture(x+1,y+1,:);
    elseif x == 1 && y == grid
        result = -1*advectionTexture(x+1,y-1,:);
    elseif x == grid && y == 1
        result = -1*advectionTexture(x-1,y+1,:);
    elseif x == grid && y == grid
        result = -1*advectionTexture(x-1,y-1,:);
    elseif x == 1
%         -1*advectionTexture(x+1,y)
        result = -1*advectionTexture(x+1,y,:);
    elseif x == grid
%         -1*advectionTexture(x-1,y)
        result = -1*advectionTexture(x-1,y,:);
    elseif y == 1
%         -1*advectionTexture(x,y+1)
        result = -1*advectionTexture(x,y+1,:);
    elseif y == grid
%         -1*advectionTexture(x,y-1)
        result = -1*advectionTexture(x,y-1,:);
    else

    x1 = floor(X);
    x2 = ceil(X);
    y1 = floor(Y);
    y2 = ceil(Y);
    if(rem(X,1) == 0 && rem(Y,1) == 0)
        result = advectionTexture(X,Y,:);
    elseif (rem(X,1) == 0)
        result = ((y2-Y)/(y2-y1))*advectionTexture(X,y1,:) + ((Y-y1)/(y2-y1))*advectionTexture(X,y2,:);
    
    elseif (rem(Y,1) == 0)
        result = ((x2-X)/(x2-x1))*advectionTexture(x1,Y,:) + ((X-x1)/(x2-x1))*advectionTexture(x2,Y,:);
    else
        R1 = ((x2-X)/(x2-x1))*advectionTexture(x1,y1,:) + ((X-x1)/(x2-x1))*advectionTexture(x2,y1,:);
        R2 = ((x2-X)/(x2-x1))*advectionTexture(x1,y2,:) + ((X-x1)/(x2-x1))*advectionTexture(x2,y2,:);
        result = ((y2-Y)/(y2-y1))*R1 + ((Y-y1)/(y2-y1))*R2;
    end
    
    end
end

% Adding force: u = u + f
function [result] = addForces(x, y, forces, newVelocityTexture, newT, T0, newRho)
    alpha = 1;
    beta = 1;
    a(1,1,1) = -alpha*newRho(x,y) + beta*(newT(x,y)-T0);
%     f_bouy = -alpha*smoke_rho*[0 1] + beta*(Temper-T0)*[0 1];
%     result = newVelocityTexture(x, y,:) + forces(x, y,:) + a;
    result = newVelocityTexture(x, y,:) + forces(x, y,:) + a;
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
% SKA DET VARA P(X+1,Y) - P(X,Y)/ H ELLER P(X+1,Y) - P(X,Y)/ 1 ??????
function [result] = gradientSubtraction(x, y, u, p, dx, rho)
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
    uNew(x, y, :) = u(x, y,:);
    Ux = uNew(x,y,1);
    Uy = uNew(x,y,2);
    U = [Ux; Uy];
%     ux = newVelo(x,y,1);
%     uy = newVelo(x,y,2);
%     newV = [ux;uy];
    final = U - diff/rho;
    result = final;
end