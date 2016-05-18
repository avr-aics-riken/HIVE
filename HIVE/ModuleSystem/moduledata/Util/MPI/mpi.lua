MPI = {}
setmetatable(MPI, {__index = HiveBaseModule})

MPI.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=MPI})
    return this
end

function MPI:Do()
    -- nothing to do
    self:UpdateValue()
    local v = self.value
    return true
end

function MPI:mpimode()
    return mpiMode()
end

function MPI:mpirank()
    return mpiRank()
end

function MPI:mpisize()
    return mpiSize()
end

function MPI:platform()
    return platform()
end

function MPI:dllextension()
    return dllExtension()
end

function MPI:endian()
    return endian()
end