function py()
    angle = 137.5;
    in_x = [];
    in_y = [];
    for n = 1 : 1000
        r = sqrt(n);
        x = ceil(cos(n*137.5)*r*5);
        y = ceil(sin(n*137.5)*r*5);
        try
            in_x = [in_x ; x];
            in_y = [in_y ; y];
        catch
            in_x = x;
            in_y = y;
        end
    end
    plotmatrix(in_x,in_y);
end