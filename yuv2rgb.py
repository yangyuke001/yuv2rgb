from numpy import *
from PIL import Image

def readYuvFile(filename,width,height):
    fp=open(filename,'rb')
    uv_width=width//2
    uv_height=height//2

    Y=zeros((height,width),uint8,'C')
    U=zeros((uv_height,uv_width),uint8,'C')
    V=zeros((uv_height,uv_width),uint8,'C')

    for m in range(height):
        for n in range(width):
            Y[m,n]=ord(fp.read(1))
    for m in range(uv_height):
        for n in range(uv_width):
            V[m,n]=ord(fp.read(1))
            U[m,n]=ord(fp.read(1))

    fp.close()
    return (Y,U,V)

def yuv2rgb(Y,U,V,width,height):
    U=repeat(U,2,0)
    U=repeat(U,2,1)
    V=repeat(V,2,0)
    V=repeat(V,2,1)
    rf=zeros((height,width),float,'C')
    gf=zeros((height,width),float,'C')
    bf=zeros((height,width),float,'C')

    #rf=Y+1.4075*(V-128.0)
    #gf=Y-0.3455*(U-128.0)-0.7169*(V-128.0)
    #bf=Y+1.779*(U-128.0)
    #rf = 1.164*Y + 1.596 * V - 222.9
    #gf = 1.164*Y - 0.392 * U - 0.823 * V+ 135.6
    #bf = 1.164*Y + 2.017 * U- 276.8
    rf = Y + 1.402*(V-128.0)
    gf = Y - 0.34414*(U-128.0) - 0.71414*(V-128.0)
    bf = Y + 1.772*(U-128.0)

    
    for m in range(height):
        for n in range(width):
            if(rf[m,n]>255):
                rf[m,n]=255
            if(gf[m,n]>255):
                gf[m,n]=255
            if(bf[m,n]>255):
                bf[m,n]=255
    
    r=rf.astype(uint8)
    g=gf.astype(uint8)
    b=bf.astype(uint8)
    return (r,g,b)

if __name__=='__main__':
    width=1920
    height=1080
    data=readYuvFile('./file_1533731242565.yuv',width,height)
    Y=data[0]
    im=Image.frombytes('L',(width,height),Y.tostring())
    im.save('./file_1533731242565.bmp')
    #im.show()

    RGB=yuv2rgb(data[0],data[1],data[2],width,height)
    im_r=Image.frombytes('L',(width,height),RGB[0].tostring())
    im_g=Image.frombytes('L',(width,height),RGB[1].tostring())
    im_b=Image.frombytes('L',(width,height),RGB[2].tostring())
    im_rgb=Image.merge('RGB',(im_r,im_g,im_b))
    im_rgb.save('./file_1533731242565.jpg')
    im_rgb.show()
