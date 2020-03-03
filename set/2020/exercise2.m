prog='exercise2';
nt=[1,2,4,8];

if ispc
    cpuName=winqueryreg('HKEY_LOCAL_MACHINE', ...
                      'HARDWARE\DESCRIPTION\System\CentralProcessor\0', ...
                      'ProcessorNameString');
else
    command='awk -F: ''/model name/{print $2; exit}'' /proc/cpuinfo';
    [~,cpuName]=system(command);
end
cores=evalc('feature(''numcores'');');
cores=strsplit(cores);
numcores=str2double(cores{3});
numcpus=str2double(cores{8});

fprintf('CPU name:       %s\n',cpuName)
fprintf('physical cores: %d\n',numcores)
fprintf('logical cores:  %d\n',numcpus)

len=length(nt);
t=zeros(len,1);
um=zeros(len,1);

fprintf('N\t|u(Nx/2,Ny/2)\t|T(N)\n')
fprintf('---\t|------------\t|---\n')
for i=1:len
    [~,cmdout]=system(sprintf('%s %d',prog,nt(i)));
    cmdout=strsplit(cmdout);
    time_txt=cmdout{4};
    t(i)=str2double(time_txt(1:(end-1)));
    um_txt=cmdout{7};
    um(i)=str2double(um_txt(1:(end-1)));
    fprintf('%d\t|%f\t|%f\n',nt(i),um(i),t(i))
end

ps=t(1)./t;
pe=100*ps./nt';

figure
subplot(2,1,1)
plot(nt,ps,'.-')
xlabel('N (threads)')
ylabel('S(N) (speedup)')
title('parallel speedup, S(N) = T(1)/T(N)')

subplot(2,1,2)
plot(nt,pe,'.-')
xlabel('N (threads)')
ylabel('E(N) (efficiency)')
title('parallel efficiency, E(N) = 100*T(1)/T(N)/N [%]')

saveas(gcf,'parallel_graph.png','png')