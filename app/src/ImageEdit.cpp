#include "ImageEdit.h"

void setNewImage(char *audioPath, char *filePath) {
    TagLib::String fileName = audioPath;
    TagLib::String fileType = fileName.substr(fileName.size() - 3).upper();

    ImageFile imageFile(filePath);
    if (fileType == "MP3") {
        TagLib::MPEG::File audioFile(audioPath);

        TagLib::ID3v2::Tag *tag = audioFile.ID3v2Tag(true);
        TagLib::ID3v2::AttachedPictureFrame *frame = new TagLib::ID3v2::AttachedPictureFrame;

        frame->setMimeType("image/jpeg");
        frame->setPicture(imageFile.data());

        tag->addFrame(frame);
        audioFile.save();
    }
}
