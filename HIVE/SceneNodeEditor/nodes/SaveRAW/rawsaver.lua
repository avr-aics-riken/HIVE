function SaveRAW(property)
	local volSaver = require("RawVolumeSaver")()()
	local appendFooter = false
	volSaver:SetVolumeData(property.volumedata)
	volSaver:Save(property.filepath, appendFooter)
end
