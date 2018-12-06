function plot3DArray( x , y , z )
    surf(x,y,z)
    xlabel('X axis')
    ylabel('Y axis')
    zlabel('Z axis')
    %shading interp
    colorbar
end
