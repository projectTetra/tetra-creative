#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <GL/glew.h>

#include <vector>

namespace tetra
{
    /**
     * This class represents an OpenGL Buffer.
     */
    template <class Data>
    class Buffer
    {
    public:
        enum BindTarget
        {
            /** Vertex Attributes */
            Array = GL_ARRAY_BUFFER,
            /** Atomic counter Storage -- Min version 4.2  */
            AtomicCounter = GL_ATOMIC_COUNTER_BUFFER,
            /** Buffer copy source */
            CopyRead = GL_COPY_READ_BUFFER,
            /** Buffer copy destination */
            CopyWrite = GL_COPY_WRITE_BUFFER,
            /** Indirect compute dispatch commands -- Min version 4.3 */
            DispatchIndirect = GL_DISPATCH_INDIRECT_BUFFER,
            /** Indirect command arguments */
            DrawIndirect = GL_DRAW_INDIRECT_BUFFER,
            /** Vertex array indices */
            ElementArray = GL_ELEMENT_ARRAY_BUFFER,
            /** Pixel read target */
            PixelPack = GL_PIXEL_PACK_BUFFER,
            /** Texture data source */
            PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
            /** Query result buffer -- Min version 4.4 */
            Query = GL_QUERY_BUFFER,
            /** Read-write storage for shaders -- Min version 4.3 */
            ShaderStorage = GL_SHADER_STORAGE_BUFFER,
            /** Texture data buffer */
            Texture = GL_TEXTURE_BUFFER,
            /** Transform feedback buffer */
            TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
            /** Uniform block storage */
            Uniform = GL_UNIFORM_BUFFER
        };

        /**
         * UsageHint is a hint to the GL implementation as to how a buffer object's
         * data store will be accessed.
         * The hint is a suggestion to the GL, but it does not constrain the buffer's
         * actual usage.
         * There are two parts: first the frequency of access, and second the nature
         * of the access.
         * There are three frequencies:
         *   Stream - The data will be modified once and used at most a few times
         *   Static - The data will be modified once and used many times
         *   Dynamic - The data will be modified repeatedly and used many times
         * There are three access types:
         *   Draw - The data is modified by the application and used by the GL
         *   Read - The data is modified by the GL and used by the application
         *   Copy - The data is modified by the GL and used by the GL
         * See documentation for glBufferData for further breakdown.
         */
        enum UsageHint
        {
            StreamDraw = GL_STREAM_DRAW,
            StreamRead = GL_STREAM_READ,
            StreamCopy = GL_STREAM_COPY,

            StaticDraw = GL_STATIC_DRAW,
            StaticRead = GL_STATIC_READ,
            StaticCopy = GL_STATIC_COPY,

            DynamicDraw = GL_DYNAMIC_DRAW,
            DynamicRead = GL_DYNAMIC_READ,
            DynamicCopy = GL_DYNAMIC_COPY,
        };

        /**
         * Create a OpenGL buffer object.
         */
        Buffer(BindTarget target);

        /**
         * Destroy the OpenGL buffer object.
         */
        ~Buffer();

        /**
         * The Buffer implementation does not support copying buffer objects.
         */
        Buffer(const Buffer&) = delete;

        /**
         * Transfer ownership of the OpenGL buffer object.
         */
        Buffer(Buffer&& from);

        /**
         * Bind the buffer to a new target.
         */
        void bind(BindTarget target);

        /**
         * Get a non-owning reference to the raw OpenGL buffer object.
         */
        GLuint raw() const;

        /**
         * Write data into the GL buffer.
         * Automatically bind the buffer to it's last bound target.
         */
        void write(const std::vector<Data>& data,
                   UsageHint usage = UsageHint::StreamDraw);

        /**
         * Read data out of the GL buffer.
         * The data is undefined if the Data type does not match what is stored in
         * the buffer.
         * Automatically bind the buffer to it's last bound target.
         * @throws GLException
         *     if sizeof(Data)*offset + sizeof(Data)*size is greater
         *     than the size of the GL buffer
         */
        std::vector<Data> read(int offset, int size);

    private:
        BindTarget target;
        GLuint handle;
    };
} /* namespace tetra */

#endif
