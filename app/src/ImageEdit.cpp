#include "ImageEdit.h"

void setNewImage(char *audioPath, char *filePath) {
    TagLib::String fileName = audioPath;
    TagLib::String fileType = fileName.substr(fileName.size() - 3).upper();

    ImageFile imageFile(filePath);
    if (fileType == "MP3") {
        TagLib::MPEG::File audioFile(audioPath);

        TagLib::ID3v2::Tag *tag = audioFile.ID3v2Tag(true);
        TagLib::ID3v2::FrameList frames = tag->frameList("APIC");
        TagLib::ID3v2::AttachedPictureFrame* frame = nullptr;

        if (frames.isEmpty())
        {
            frame = new TagLib::ID3v2::AttachedPictureFrame;
        }
        else
        {
            frame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frames.front());
            tag->removeFrame(frame, false);
        }

        frame->setMimeType("image/jpeg");
        frame->setPicture(imageFile.data());

        tag->addFrame(frame);
        audioFile.save();
    }
}
