i1 = imread('pout.tif');

i2 = MatlabInterfaceTest(i1); 
subplot(2,1,1); imshow(i1); subplot(2,1,2); imshow(i2);

i1 = single(i1);
i2 = MatlabInterfaceTest(i1);
i1 = double(i1)/255;
i2 = MatlabInterfaceTest(i1);
i1 = uint16(i1);
i2 = MatlabInterfaceTest(i1);
i1 = uint64(i1);
i2 = MatlabInterfaceTest(i1);
i1 = int64(i1);
i2 = MatlabInterfaceTest(i1);
i1 = repmat(i1,[1 1 3]);
i2 = MatlabInterfaceTest(i1);