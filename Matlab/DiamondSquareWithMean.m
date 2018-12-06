function test_land2(land2)
figure
x = land2;
maxstep=6;
showit(x);
disp('1');
h = 0.5;
range=0.5; 
for step=2:maxstep
    x = diamond_square_algorithm(x,range,step,maxstep);
    range = range*h*2^(-h); 
end
showit(x);
x = mean(x);
showit(x);
csvwrite('testfileexcel.csv',x);
end

function test = mean(x)
    size_x = size(x);
    size_x = size_x - 1;
    test = x;
    for i = 2 : size_x(1)
        for j = 2 : size_x(2)
            test(i,j) = x(i+1,j+1) + x(i+1,j) + x(i+1,j-1) + x(i,j+1) + x(i,j) + x(i,j-1) + x(i-1,j+1) + x(i-1,j) + x(i-1,j-1);
            test(i,j) = test(i,j)/9;
        end
    end
end

function y = diamond_square_algorithm(x,range,step,maxstep)
x = addborders(x);
y = enlarge(x);
y = centerofsquares(y,range,step,maxstep);
y = wrap(y);
y = centerofdiamonds(y,range,step,maxstep);
y = removeborders(y);
showit(y);
a = size(y);
disp(strcat(num2str(step),'=',num2str(a(1)),':',num2str(a(2))));
end

function y = addborders(x)
y = zeros(size(x)+2);
y(2:end-1,2:end-1)=x;
end

function y = enlarge(x)
n = size(x,1);
y = zeros(2*n-3,2*n-3);
for i=2:n-1
    for j=2:n-1
        y(2*i-2,2*j-2)=x(i,j);
    end
end
end

function y = centerofsquares(x,range,step,maxstep)
n=size(x,1);
y = x;
for i=3:2:n-2
    for j=3:2:n-2
        y(i,j) = (y(i-1,j-1)+y(i+1,j+1)+y(i+1,j-1)+y(i-1,j+1))*0.25 + 2*range*rand-range;
    end
end
end

function y=wrap(x)
y = x;
y(:,1)   = x(:,3); 
y(:,end) = x(:,end-2)    ; 
y(1,:)   = x(3,:);
y(end,:) = x(:,end-2)    ;
end

function y = centerofdiamonds(x,range,step,maxstep);
n=size(x,1);
y = x;
for i=2:1:n-1
    for j=3-mod(i,2):2:n-1
        y(i,j) = (y(i-1,j)+y(i+1,j)+y(i,j-1)+y(i,j+1))*0.25+2*range*rand-range;
    end
end
end

function y = removeborders(x)
y=x(2:end-1,2:end-1);
end

function showit(x,step,maxstep,scan,maxscan,phase)
pause(1);
size_x = size(x);
token = [1:size_x(1)];
p_X = token;
for index = 2 : size_x(2)
    p_X = [p_X;token];
end
p_Y = p_X';
p_Z = x;
plot3DArray( p_X , p_Y , p_Z );
end

