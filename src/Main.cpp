#include <stdio.h>
namespace Radon{
enum Scheme{bin,hex8bit,hex4bit,hex16bit};
enum Type{read,roverwrite,rappend,overwrite,append};

template<class T>
class Stream{
private:
	Scheme m_scheme;
	FILE* m_file;
public:
	Stream(char* file,Scheme scheme,Type type)
{
		m_scheme = scheme;

		switch(scheme)
		{
		case bin:
			switch(type)//the b is cause we're working with binary data
			{
			case rappend:
				m_file = fopen(file,"rb+");
				break;
			case roverwrite:
				m_file = fopen(file,"wb+");
				break;
			case read:
				m_file = fopen(file,"rb");
				break;
			case append:
				m_file = fopen(file,"ab");
				break;
			case overwrite:
				m_file = fopen(file,"wb");
				break;
			}
			break;
		}
}

	~Stream()
	{
		destroy();
	}

	void destroy()
	{
		fclose(m_file);
		delete m_file;
	}

	bool serialize(T data)
	{
	   fwrite(&data,sizeof(T),sizeof(data),m_file);
		if (0==ferror(m_file))
			return true;
		else
			return false;
	}

	T deserialize()
	{
		rewind(m_file);
		fseek (m_file , 0 , SEEK_END);
		long lSize = ftell (m_file);
		rewind (m_file);

		T* result = new T();//(T) malloc(sizeof(T)*lSize);
		fread(result,sizeof(T),lSize,m_file);

		if (ferror(m_file)!=0)
			return -1;
		else
			return *result;
	}


};



//non object oriented versions of above
template<class T>
bool serialize(char* file, T data, Scheme scheme,Type type)
{
    FILE* m_file;
     switch(scheme)
    {

        case bin:
            switch(type)//the b is cause we're working with binary data
            {
            case rappend:
                m_file = fopen(file,"rb+");
                break;
            case roverwrite:
                m_file = fopen(file,"wb+");
                break;
            case read:
                m_file = fopen(file,"rb");
                break;
            case append:
                m_file = fopen(file,"ab");
                break;
            case overwrite:
                m_file = fopen(file,"wb");
                break;
            }
            break;
    }

    fwrite(&data,sizeof(T),sizeof(data),m_file);
    if (0==ferror(m_file))
    {
        fclose(m_file);
        return true;
    }else{
        fclose(m_file);
        return false;
    }


}

template<class T>
T deserialize(char* file, Scheme scheme,Type type)
{
    FILE* m_file;
	     switch(scheme)
		{

            case bin:
                switch(type)//the b is cause we're working with binary data
                {
                case rappend:
                    m_file = fopen(file,"rb+");
                    break;
                case roverwrite:
                    m_file = fopen(file,"wb+");
                    break;
                case read:
                    m_file = fopen(file,"rb");
                    break;
                case append:
                    m_file = fopen(file,"ab");
                    break;
                case overwrite:
                    m_file = fopen(file,"wb");
                    break;
                }
                break;
        }

    rewind(m_file);
    fseek (m_file , 0 , SEEK_END);
    long lSize = ftell (m_file);
    rewind (m_file);

    T* result = new T();//(T) malloc(sizeof(T)*lSize);
    fread(result,sizeof(T),lSize,m_file);

    if (ferror(m_file)!=0)
    {
        fclose(m_file);
        return -1;
    }
    else
    {
        fclose(m_file);
        return *result;
    }
}



};

int main()
{
	/*
	 * Radon is a serialization library.
	 */
	int b = 1030404;

	Radon::Stream<int> stream ((char*)"serialized.ser",Radon::bin,Radon::roverwrite);

	stream.serialize(b);

	int b2 = stream.deserialize();
	printf("%i should = %i (samestream)\n",b,b2);
	stream.destroy();


	Radon::Stream<int> exstream ((char*)"serialized.ser",Radon::bin,Radon::read);
	int a = exstream.deserialize();
	exstream.destroy();

	printf("%i should = %i\n",a,b);


	float nonoop = 13245.678f;
	Radon::serialize<float>((char*)"nonoop.ser",nonoop,Radon::bin,Radon::roverwrite);

    float result = Radon::deserialize<float>((char*)"nonoop.ser",Radon::bin,Radon::read);
    printf("%f should match %f (nonoop)\n",result,nonoop);

}
