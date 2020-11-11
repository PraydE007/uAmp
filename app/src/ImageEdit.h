#ifndef UAMP_IMAGEEDIT_H
#define UAMP_IMAGEEDIT_H

// TAGLIB
#include <../../3dparty/lib1/mpegfile.h>
#include <../../3dparty/lib1/attachedpictureframe.h>
#include <../../3dparty/lib1/id3v2tag.h>

// C++
#include <iostream>

class ImageFile : public TagLib::File
{
public:
    ImageFile(const char *file) : TagLib::File(file) {}

    TagLib::ByteVector data() {
        return readBlock(length());
    }

private:
    virtual TagLib::Tag *tag() const { return 0; }
    virtual TagLib::AudioProperties *audioProperties() const { return 0; }
    virtual bool save() { return false; }
};

void setNewImage(char *audioFile, char *filePath);

#endif //UAMP_IMAGEEDIT_H
