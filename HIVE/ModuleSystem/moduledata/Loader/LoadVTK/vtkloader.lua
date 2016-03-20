LoadVTK = {}
setmetatable(LoadVTK, {__index = HiveBaseModule})

LoadVTK.new = function (varname)
    local this = HiveBaseModule.new(varname);
    this.loader = require('VtkLoader')()
    setmetatable(this, {__index=LoadVTK})
    return this
end

function LoadVTK:Do()
    self:UpdateValue()
    return self.loader:Load(self.value.filepath, self.value.dirpath, self.value.fieldname, true)
end

function LoadVTK:VolumeData()
    return self.loader:VolumeData()
end

function LoadVTK:Size()
    local size = {self.loader:Width(), self.loader:Height(), self.loader:Depth()}
    return size
end

function LoadVTK:Component()
    return self.loader:Component()
end

function LoadVTK:Pitch()
	local pitch = {self.loader:PitchX(), self.loader:PitchY(), self.loader:PitchZ()}
	return pitch
end

function LoadVTK:Origin()
	local origin = {self.loader:OriginX(), self.loader:OriginY(), self.loader:OriginZ()}
	return origin
end

function LoadVTK:Time()
	return self.loader:Time()
end

function LoadVTK:Step()
	return self.loader:Step()
end
