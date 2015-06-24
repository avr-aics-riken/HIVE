function MPI_Info(arg)
	return {
		rank = function()
			return mpiRank()
		end,
		size = function()
			return mpiSize()
		end
	}
end