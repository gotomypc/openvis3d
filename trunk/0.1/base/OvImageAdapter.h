#ifndef __OVIMAGEADAPTER_H
#define __OVIMAGEADAPTER_H

class OvImageAdapter
{
public:
	OvImageAdapter();
	virtual ~OvImageAdapter();

	enum OvDataType {
		OV_DATA_UNKNOWN, 
		OV_DATA_UINT8, 
		OV_DATA_INT8, 
		OV_DATA_UINT16, 
		OV_DATA_INT16, 
		OV_DATA_UINT32, 
		OV_DATA_INT32, 
		OV_DATA_UINT64, 
		OV_DATA_INT64, 
		OV_DATA_FLOAT32, 
		OV_DATA_DOUBLE64
	};

	virtual void getSize(int & height, int & width, int & nColorChannels);
	virtual void getDataType(OvImageAdapter::OvDataType & dataType);

	virtual void getPixel(float * value, int row, int column) = 0;
	virtual void setPixel(float * value, int row, int column) = 0;


protected:
	int  mHeight;			//height of the image
	int  mWidth;			//width of the image
	int  mChannels;			//number of color channels (e.g., 1 for grayscale, 3 for RGB)
	OvDataType mDataType;   //data format of a pixel channel
};

#endif
