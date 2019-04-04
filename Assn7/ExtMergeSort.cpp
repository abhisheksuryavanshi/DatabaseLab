#include "common.h"
#include "ExtMergeSort.h"

//creates initial runs of 1 page size
void ExtMergeSort :: firstPass(DiskFile &inputFile, MainMemory &memory){
	
	int frame = -1;
	// load each page to main memory frame and sort
	for(int i = 0; i < inputFile.totalPages; i++){
		frame = memory.loadPage(inputFile, i);
		this->sortFrame(memory, frame);
		memory.writeFrame(inputFile, frame, i);
		memory.freeFrame(frame);
	}
	
	runSize = 1;
	totalPass = 1;
	totalRuns = inputFile.totalPages;
	cout << "First Pass Performed" << endl;
	inputFile.writeDiskFile(); //print file to cout
}

//sorts each frame
void ExtMergeSort :: sortFrame(MainMemory &memory, int f){
	sort(memory.data[f].arr.begin(), memory.data[f].arr.begin()+memory.getValidEntries(f));
}

//Performs merging of 2 runs
void ExtMergeSort :: merge(DiskFile &inputFile, MainMemory &memory, int leftStart, int mid, int rightEnd){

	int finalRunSize = rightEnd - leftStart + 1;
	DiskFile tempFile(finalRunSize);

	bool flag = false;
	int currPage = 0;
	int l = leftStart;
	int r = mid + 1;


	cout<<"Loading: "<<l<<endl;
	int leftFrame = memory.loadPage(inputFile, l);
	cout<<"Loading: "<<r<<endl;
	int rightFrame = memory.loadPage(inputFile, r);
	int resFrame = memory.getEmptyFrame();	//frame to store result

	int leftbuffer = -1;
	int ltemp = -1;
	bool lback = true;
	if(l+1<= mid)
	{
		cout<<"Loading: "<<l<<endl;
		leftbuffer = memory.loadPage(inputFile, ++l);
		lback = false;
	}

	int rightbuffer = -1;
	int rtemp = -1;
	bool rback = true;
	if(r+1<=rightEnd)
	{
		cout<<"Loading: "<<r<<endl;
		rightbuffer = memory.loadPage(inputFile, ++r);
		rback = false;
	}

	
	if(leftFrame == -1 || rightFrame == -1 || resFrame == -1)
	{
		cout << "Can't proceed further due to unavailability of memory or invalid Page access" << endl;
		exit(1); 
	}

	int leftIndex = 0;
	int rightIndex = 0;
	int resIndex = 0;
	

	while(l <= mid && r <= rightEnd)
	{
		if(leftIndex < memory.getValidEntries(leftFrame) && rightIndex < memory.getValidEntries(rightFrame))
		{	
			int x = memory.getVal(leftFrame, leftIndex);
			int y = memory.getVal(rightFrame, rightIndex); 
			cout<<"Comparing: "<<x<<" and "<<y<<endl;
			if( x < y)
			{
				memory.setVal(resFrame, resIndex, x);
				flag = true;
				leftIndex++;
			}
			else
			{
				flag = true;
				memory.setVal(resFrame, resIndex, y);
				rightIndex++;
			}
			resIndex++;
			if(resIndex == MEM_FRAME_SIZE)
			{
				memory.writeFrame(tempFile, resFrame, currPage);
				// cout<<"TEMP FILE:"<<endl;
				// tempFile.writeDiskFile();
				flag = false;
				memory.freeFrame(resFrame);
				resFrame = memory.getEmptyFrame();
				currPage++; 
				resIndex = 0;
			}
		}

		if(leftIndex == memory.getValidEntries(leftFrame))
		{
			if(!lback)
			{
				cout<<"Using left backup"<<endl;
				leftIndex = 0;
				ltemp = leftFrame;
				leftFrame = leftbuffer;
				lback = true;
			}
			else
			{
				memory.freeFrame(leftFrame);
				if(ltemp != -1)
					memory.freeFrame(ltemp);
				l++;
				if(l <= mid)
				{
					leftFrame = memory.loadPage(inputFile, l);
					if(l+1 <= mid)
					{
						leftbuffer = memory.loadPage(inputFile, ++l);
						lback = false;
					}
					leftIndex = 0;
				}
			}
		}

		if(rightIndex == memory.getValidEntries(rightFrame))
		{
			if(!rback)
			{
				cout<<"Using right backup"<<endl;
				rightIndex = 0;
				rtemp = rightFrame;
				rightFrame = rightbuffer;
				rback = true;
			}
			else
			{
				memory.freeFrame(rightFrame);
				if(rtemp != -1)
					memory.freeFrame(rtemp);
				r++;
				
				if(r <= rightEnd)
				{
					rightFrame = memory.loadPage(inputFile, r);
					if(r+1 <= rightEnd)
					{
						rback = false;
						rightbuffer = memory.loadPage(inputFile, ++r);
					}
					rightIndex = 0;
				}
			}	
		}
	}
	while(l <= mid)
	{
		int x = memory.getVal(leftFrame, leftIndex);
		memory.setVal(resFrame, resIndex, x);
		flag = true;
		leftIndex++;
		resIndex++;
		if(resIndex == MEM_FRAME_SIZE)
		{
			memory.writeFrame(tempFile, resFrame, currPage);
			flag = false;
			memory.freeFrame(resFrame);
			resFrame = memory.getEmptyFrame();
			currPage++; 
			resIndex = 0;
		}
		if(leftIndex == memory.getValidEntries(leftFrame))
		{
			if(!lback)
			{
				cout<<"Using left backup"<<endl;
				leftIndex = 0;
				ltemp = leftFrame;
				leftFrame = leftbuffer;
				lback = true;
			}
			else
			{
				memory.freeFrame(leftFrame);
				if(ltemp != -1)
					memory.freeFrame(ltemp);
				l++;
				if(l <= mid)
				{
					leftFrame = memory.loadPage(inputFile, l);
					if(l+1 <= mid)
					{
						leftbuffer = memory.loadPage(inputFile, ++l);
						lback = false;
					}
					leftIndex = 0;
				}
			}	

		}
	}
	while(r <= rightEnd)
	{
		int x = memory.getVal(rightFrame, rightIndex);
		memory.setVal(resFrame, resIndex, x);
		flag = true;
		rightIndex++;
		resIndex++;
		if(resIndex == MEM_FRAME_SIZE)
		{
			memory.writeFrame(tempFile, resFrame, currPage);
			flag = false;
			memory.freeFrame(resFrame);
			resFrame = memory.getEmptyFrame();
			currPage++; 
			resIndex = 0;
		}
		if(rightIndex == memory.getValidEntries(rightFrame))
		{
			if(!rback)
			{
				cout<<"Using right backup"<<endl;
				rightIndex = 0;
				rtemp = rightFrame;
				rightFrame = rightbuffer;
				rback = true;
			}
			else
			{
				memory.freeFrame(rightFrame);
				if(rtemp != -1)
					memory.freeFrame(rtemp);
				r++;
				
				if(r <= rightEnd)
				{
					rightFrame = memory.loadPage(inputFile, r);
					if(r+1 <= rightEnd)
					{
						rback = false;
						rightbuffer = memory.loadPage(inputFile, ++r);
					}
					rightIndex = 0;
				}
			}	

		}
	}
	if(flag)
		memory.writeFrame(tempFile, resFrame, currPage);
	memory.freeFrame(resFrame);
	memory.freeFrame(leftFrame);
	memory.freeFrame(rightFrame);
	inputFile.DiskFileCopy(tempFile, leftStart, rightEnd);
	inputFile.writeDiskFile();

}

//Performs 2 way merge sort on inputFile using memory
void ExtMergeSort :: twoWaySort(DiskFile &inputFile, MainMemory &memory){
	
	if(memory.totalFrames < 3)
		cout << "Error: Two way merge sort requires atleast 3 frames" << endl; 
	
	this->firstPass(inputFile, memory);

	int leftStart;
	
	for(this->runSize = 1; this->runSize < inputFile.totalPages; this->runSize *= 2)
	{
		cout << "runSize: " << this->runSize << endl;
		for(leftStart = 0; leftStart < inputFile.totalPages-1; leftStart += 2*this->runSize)
		{
			int mid = leftStart + this->runSize-1;
			int rightEnd = min(leftStart + 2*this->runSize-1, inputFile.totalPages-1);
			cout << "calling merge for < " << leftStart <<", " << mid << ", " << rightEnd << " >" << endl;
			this->merge(inputFile, memory, leftStart, mid, rightEnd);
		}
		totalPass++;
	}

	cout << "Total Passes required: " << totalPass << endl;
}

