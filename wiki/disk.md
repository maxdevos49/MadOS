
### Disk Vocabulary

#### Sectors
A "Sector" refers to a group of 512 contiguous bytes. Modern day disks can have sector sizes of 2048 contiguous bytes or more.

#### Head 
A "Head" (or Face) represents the literal side of the disk.

#### Track or Cylinder
A "Track" is one one ring around a disk. A track is also referred to as a Cylinder. In a floppy there are 18 sectors per track

#### CHS
A form of addressing the location of data typically on older disk like a floppy. The location is indexed as [Cylinder][Head][Sector]. CHS addressing is obsolete.

#### LBA
"Linear Block Address" is modern form of addressing the location of data on disk of all types in a linear manner. LBA is considered linear because LBA + 1 will be the next sector on the disk. Calculating the LBA depends upon the geometry of the disk.

#### Disk Geometry
A Disk's geometry is defined by 