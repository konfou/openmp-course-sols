prog='exercise1';
Nx=40;
Ny=40;
tp=[0,5,10,20];
system(sprintf('%s',prog));
data_n=load('wave2d_numeric.dat');
data_a=load('wave2d_analytic.dat');

for t=tp
    figure

    %numerical surf
    d=data_n(data_n(:,1)==t,:);
    X=reshape(d(:,2),Nx,Ny);
    Y=reshape(d(:,3),Nx,Ny);
    Z=reshape(d(:,4),Nx,Ny);

    subplot(2,1,1)
    surf(X,Y,Z)
    xlabel('x')
    ylabel('y')
    zlabel('u(x,y)')
    title(sprintf('numerical results, t=%f',t))

    %analytical surf
    d=data_a(data_a(:,1)==t,:);
    X=reshape(d(:,2),Nx,Ny);
    Y=reshape(d(:,3),Nx,Ny);
    Z=reshape(d(:,4),Nx,Ny);
    
    subplot(2,1,2)
    surf(X,Y,Z)
    xlabel('x')
    ylabel('y')
    zlabel('u(x,y)')
    title(sprintf('analytical results, t=%f',t))
    
    saveas(gcf,sprintf('surf_t_%f.png',t),'png')
end