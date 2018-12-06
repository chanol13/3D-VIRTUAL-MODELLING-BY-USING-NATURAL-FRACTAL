function testangle
    x = rand()*10 - 5;
    y = rand()*10 - 5;
    z = rand()*10 - 5;
    point = [x,y,z]
    rad = x*x + y*y + z*z;
    rad = sqrt(rad)
    radz = x*x + y*y;
    radz = sqrt(radz);
    angleZAxis = 30;
    newx = rad*acos(x/radz);
    newy = rad*asin(x/radz);
    x = newx;
    y = newy;
    z = z;
    point = [x,y,z]
    rad = x*x + y*y + z*z;
    rad = sqrt(rad)
end