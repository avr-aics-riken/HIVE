LoadPDB = {}
setmetatable(LoadPDB, {__index = HiveBaseModule})

LoadPDB.new = function (varname)
    local this = HiveBaseModule.new(varname);
    this.loader = LoadModule('PdbLoader')
    setmetatable(this, {__index=LoadPDB})
    return this
end

function LoadPDB:Do()
    self:UpdateValue()
    return self.loader:Load(self.value.filepath,'true')
end

function LoadPDB:BallData()
    return self.loader:BallData()
end

function LoadPDB:StickData()
    return self.loader:StickData()
end
