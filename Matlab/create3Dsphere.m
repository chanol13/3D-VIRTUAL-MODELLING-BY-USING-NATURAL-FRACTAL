function create3Dsphere()
    [x,y,z] = sphere(100);
    figure
    surf(x*100,y*100,z*100);
end

