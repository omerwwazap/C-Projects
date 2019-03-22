int findColdest(int day[])
{
	int	i, minIndx;

	minIndx = 0;
	for (i = 1; i<WEEK; i++)
		if (day[i] < day[minIndx])
			minIndx = i;

	return minIndx;
}
