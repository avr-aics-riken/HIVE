LoadCDMMx1 = {}
setmetatable(LoadCDMMx1, {__index = HiveBaseModule})

LoadCDMMx1.new = function (varname)
    local this = HiveBaseModule.new(varname);
    this.loader = require('CdmLoader')()
    setmetatable(this, {__index=LoadCDMMx1})
    return this
end

function LoadCDMMx1:Do()
    self:UpdateValue()
	if self.value.filepath ~= nil and self.value.filepath ~= "" then
       return self.loader:LoadMx1(self.value.filepath, self.value.time)
	else
		return false
	end
end

function LoadCDMMx1:VolumeData()
    return self.loader:VolumeData()
end

function LoadCDMMx1:Size()
    local size = {self.loader:Width(), self.loader:Height(), self.loader:Depth()}
    return size
end

function LoadCDMMx1:Component()
    return self.loader:Component()
end

function LoadCDMMx1:GlobalDiv()
    local globalDiv = {self.loader:GlobalDivX(), self.loader:GlobalDivY(), self.loader:GlobalDivZ()}
    return globalDiv
end

function LoadCDMMx1:GlobalVoxel()
    local globalVoxel = {self.loader:GlobalVoxelX(), self.loader:GlobalVoxelY(), self.loader:GlobalVoxelZ()}
    return globalVoxel
end

function LoadCDMMx1:GlobalOffset()
    local globalOffset = {self.loader:GlobalOffsetX(), self.loader:GlobalOffsetY(), self.loader:GlobalOffsetZ()}
    return globalOffset
end

function LoadCDMMx1:GlobalRegion()
    local globalRegion = {self.loader:GlobalRegionX(), self.loader:GlobalRegionY(), self.loader:GlobalRegionZ()}
    return globalRegion
end


function LoadCDMMx1:NumTimeSteps()
    return self.loader:NumTimeSteps()
end
