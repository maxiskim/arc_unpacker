#include "formats/qlie/abmp7_archive.h"
#include "util/encoding.h"
using namespace Formats::QLiE;

namespace
{
    const std::string magic("ABMP7", 5);

    void read_first_file(File &arc_file, FileSaver &file_saver)
    {
        size_t length = arc_file.io.read_u32_le();
        std::unique_ptr<File> subfile(new File);
        subfile->io.write_from_io(arc_file.io, length);
        subfile->name = arc_file.name + "$.dat";
        subfile->guess_extension();
        file_saver.save(std::move(subfile));
    }

    void read_next_file(File &arc_file, FileSaver &file_saver)
    {
        std::string encoded_name = arc_file.io.read(arc_file.io.read_u8());
        arc_file.io.skip(31 - encoded_name.size());
        std::string name = convert_encoding(encoded_name, "cp932", "utf-8");
        size_t length = arc_file.io.read_u32_le();
        std::unique_ptr<File> subfile(new File);
        subfile->io.write_from_io(arc_file.io, length);
        subfile->name = arc_file.name + "_" + name + ".dat";
        subfile->guess_extension();
        file_saver.save(std::move(subfile));
    }
}

void Abmp7Archive::unpack_internal(File &arc_file, FileSaver &file_saver) const
{
    if (arc_file.io.read(magic.size()) != magic)
        throw std::runtime_error("Not an ABMP7 container");

    arc_file.io.seek(12);
    arc_file.io.skip(arc_file.io.read_u32_le());

    read_first_file(arc_file, file_saver);
    while (arc_file.io.tell() < arc_file.io.size())
        read_next_file(arc_file, file_saver);
}
