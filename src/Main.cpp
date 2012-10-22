#include <stdio.h>
namespace Radon{
    enum Scheme{read,text,bin,sha1,bit4};

    template<class T>
    class Stream{
        private:
            Scheme m_scheme;
            FILE* m_file;
        public:
            Stream(char* file,Scheme scheme)
            {
                if (scheme!=Radon::read)
                {
                    m_scheme = scheme;
                    m_file = fopen(file,"wb");//note this overwrites 4 now
                }else{
                    m_scheme = scheme;
                    m_file = fopen(file,"rb");//read only
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

};

int main()
{
    /*
     * Radon is a serialization library.
     */
    int b = 1030404;

    Radon::Stream<int> stream ((char*)"serialized.ser",Radon::bin);

    stream.serialize(b);

    int b2 = stream.deserialize();
    printf("%i should = %i (samestream)",b,b2);
    stream.destroy();


    Radon::Stream<int> exstream ((char*)"serialized.ser",Radon::read);
    int a = exstream.deserialize();
    exstream.destroy();

    printf("%i should = %i",a,b);

}
