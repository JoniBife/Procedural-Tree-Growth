#include "Morphospace.h"
#include <math.h>

Morphospace* Morphospace::instance;

Morphospace::Morphospace(float scaleLength): scaleLength(scaleLength) {

	// The module prototypes marked as correct are designed like in the paper
	// all the others are just copies and need to be redesigned to match the paper

	modulePrototypeFunctions[0][0] = [=](BranchModule* module, bool removeFirst) -> BranchModule* {
		BranchNode* child = removeFirst ? module->root : module->root->createChild({ 0.0f, 18.0f, 0.0f }, scaleLength);
		{
			BranchNode* childA = child->createChild({ 9.0f, 8.0f, 0.0f }, scaleLength);
			{
				BranchNode* childA1 = childA->createChild({ 8.0f, 3.0f, 3.0f }, scaleLength);
				{
					BranchNode* childA1a = childA1->createChild({ 5.5f, 0.5f, 1.0f }, scaleLength);
					{
						BranchNode* childA1a1 = childA1a->createChild({ 3.0f, -1.0f, 1.0f }, scaleLength);
						{
							BranchNode* childA1a1A = childA1a1->createChild({ 2.5f, -2.0f, 1.0f }, scaleLength, true);
							module->tips.push_back(childA1a1A);
							BranchNode* childA1a1B = childA1a1->createChild({ 3.0f, 0.0f, 0.0f }, scaleLength, true);
							module->tips.push_back(childA1a1B);
						}
						BranchNode* childA1a2 = childA1a->createChild({ 3.0f, 2.5f, -2.0f }, scaleLength);
						{
							BranchNode* childA1a2A = childA1a2->createChild({ 3.0f, 0.5f, 1.0f }, scaleLength, true);
							module->tips.push_back(childA1a2A);
							BranchNode* childA1a2B = childA1a2->createChild({ 2.0f, 2.5f, -2.0f }, scaleLength, true);
							module->tips.push_back(childA1a2B);
						}
					}
					BranchNode* childA1b = childA1->createChild({ 2.0f, 2.5f, -2.0f }, scaleLength);
					{
						BranchNode* childA1b1 = childA1b->createChild({ 3.5f, 0.0f, 1.0f }, scaleLength);
						{
							BranchNode* childA1b1a = childA1b1->createChild({ 1.0f, 1.0f, 1.0f }, scaleLength, true);
							module->tips.push_back(childA1b1a);
							BranchNode* childA1b1b = childA1b1->createChild({ 2.0f, 1.0f, -1.0f }, scaleLength, true);
							module->tips.push_back(childA1b1b);
						}
						BranchNode* childA1b2 = childA1b->createChild({ 0.0f, 3.0f, -2.0f }, scaleLength);
						{
							BranchNode* childA1b2a = childA1b2->createChild({ 1.0f, 1.0f, 1.0f }, scaleLength, true);
							module->tips.push_back(childA1b2a);
							BranchNode* childA1b2b = childA1b2->createChild({ 0.5f, 2.5f, -1.5f }, scaleLength, true);
							module->tips.push_back(childA1b2b);
						}
					}
				}
				BranchNode* childA2 = childA->createChild({ 2.0f, 6.5f, -2.0f }, scaleLength);
				{
					BranchNode* childA2a = childA2->createChild({ 2.5f, 2.0f, 1.0f }, scaleLength);
					{
						BranchNode* childA2a1 = childA2a->createChild({ 2.0f, 0.5f, 0.5f }, scaleLength, true);
						module->tips.push_back(childA2a1);
						BranchNode* childA2a2 = childA2a->createChild({ 0.5f, 2.0f, -1.0f }, scaleLength, true);
						module->tips.push_back(childA2a2);
					}
					BranchNode* childA2b = childA2->createChild({ 2.0f, 5.5f, -2.0f }, scaleLength);
					{
						BranchNode* childA2b1 = childA2b->createChild({ 3.0f, 3.5f, 0.0f }, scaleLength);
						{
							BranchNode* childA2b1a = childA2b1->createChild({ 2.5f, 1.0f, -1.0f }, scaleLength, true);
							module->tips.push_back(childA2b1a);
							BranchNode* childA2b1b = childA2b1->createChild({ 1.0f, 2.5f, 0.5f }, scaleLength, true);
							module->tips.push_back(childA2b1b);
						}
						BranchNode* childA2b2 = childA2b->createChild({ 0.0f, 5.0f, 1.0f }, scaleLength);
						{
							BranchNode* childA2b2a = childA2b2->createChild({ -1.0f, 3.0f, 0.0f }, scaleLength, true);
							module->tips.push_back(childA2b2a);
							BranchNode* childA2b2b = childA2b2->createChild({ 1.0f, 3.0f, 0.5f }, scaleLength, true);
							module->tips.push_back(childA2b2b);
						}
					}
				}
			}
			BranchNode* childB = child->createChild({ -9.5f, 8.0f, 0.0f }, scaleLength);
			{
				BranchNode* childB1 = childB->createChild({ -2.0f, 10.0f, 2.0f }, scaleLength);
				{
					BranchNode* childB1a = childB1->createChild({ -0.5f, 4.0f, -1.0f }, scaleLength);
					{
						BranchNode* childB1a1 = childB1a->createChild({ 1.0f, 2.5f, -1.0f }, scaleLength);
						{
							BranchNode* childB1a1A = childB1a1->createChild({ 1.5f, 0.5f, 1.0f }, scaleLength, true);
							module->tips.push_back(childB1a1A);
							BranchNode* childB1a1B = childB1a1->createChild({ 0.5f, 2.0f, -1.0f }, scaleLength, true);
							module->tips.push_back(childB1a1B);
						}
						BranchNode* childB1a2 = childB1a->createChild({ -2.5f, 4.0f, 1.0f }, scaleLength);
						{
							BranchNode* childB1a2A = childB1a2->createChild({ -1.5f, 1.5f, -0.5f }, scaleLength, true);
							module->tips.push_back(childB1a2A);
							BranchNode* childB1a2B = childB1a2->createChild({ -2.5f, 4.0f, 1.0f }, scaleLength, true);
							module->tips.push_back(childB1a2B);
						}
					}
					BranchNode* childB1b = childB1->createChild({ -2.0f, 4.5f, 0.0f }, scaleLength);
					{
						BranchNode* childB1b1 = childB1b->createChild({ 0.0f, 2.5f, 1.0f }, scaleLength);
						{
							BranchNode* childB1b1A = childB1b1->createChild({ 1.0f, 1.5f, 1.0f }, scaleLength, true);
							module->tips.push_back(childB1b1A);
							BranchNode* childB1b1B = childB1b1->createChild({ -0.5f, 3.0f, -1.0f }, scaleLength, true);
							module->tips.push_back(childB1b1B);
						}
						BranchNode* childB1b2 = childB1b->createChild({ -2.0f, 3.0f, -1.0f }, scaleLength);
						{
							BranchNode* childB1b2A = childB1b2->createChild({ -1.0f, 3.5f, -0.5f }, scaleLength, true);
							module->tips.push_back(childB1b2A);
							BranchNode* childB1b2B = childB1b2->createChild({ -2.5f, 2.0f, -0.5f }, scaleLength, true);
							module->tips.push_back(childB1b2B);
						}
					}
				}
				BranchNode* childB2 = childB->createChild({ -8.0f, 0.0f, -1.0f }, scaleLength);
				{
					BranchNode* childB2a = childB2->createChild({ -6.0f, 2.0f, -2.0f }, scaleLength);
					{
						BranchNode* childB2a1 = childB2a->createChild({ -3.5f, 1.0f, -1.0f }, scaleLength);
						{
							BranchNode* childB2a1A = childB2a1->createChild({ -1.0f, 1.5f, -0.5f }, scaleLength, true);
							module->tips.push_back(childB2a1A);
							BranchNode* childB2a1B = childB2a1->createChild({ -2.5f, 0.5f, 0.0f }, scaleLength, true);
							module->tips.push_back(childB2a1B);
						}
						BranchNode* childB2a2 = childB2a->createChild({ -4.0f, 0.0f, 1.0f }, scaleLength);
						{
							BranchNode* childB2a2A = childB2a2->createChild({ -2.5f, 1.0f, -1.0f }, scaleLength, true);
							module->tips.push_back(childB2a2A);
							BranchNode* childB2a2B = childB2a2->createChild({ -2.0f, -1.0f, 0.5f }, scaleLength, true);
							module->tips.push_back(childB2a2B);
						}
					}
					BranchNode* childB2b = childB2->createChild({ -3.5f, -2.0f, 2.0f }, scaleLength);
					{
						BranchNode* childB2b1 = childB2b->createChild({ -0.5f, 1.0f, -0.5f }, scaleLength, true);
						module->tips.push_back(childB2b1);
						BranchNode* childB2b2 = childB2b->createChild({ -3.0f, -2.0f, 2.0f }, scaleLength);
						{
							BranchNode* childB2b2A = childB2b2->createChild({ -2.5f, -0.5f, 0.5f }, scaleLength, true);
							module->tips.push_back(childB2b2);
							BranchNode* childB2b2B = childB2b2->createChild({ -2.0f, -3.0f, 1.0f }, scaleLength, true);
							module->tips.push_back(childB2b2B);
						}
						BranchNode* childB2b3 = childB2b->createChild({ -0.5f, -1.0f, 0.5f }, scaleLength, true);
						module->tips.push_back(childB2b3);

					}
				}
			}
		}
		return module;
	};
	modulePrototypeFunctions[0][1] = [=](BranchModule* module, bool removeFirst) -> BranchModule* {
		BranchNode* child = removeFirst ? module->root : module->root->createChild({ 0.0f, 18.0f, 0.0f }, scaleLength);
		child->main = true;
		{
			BranchNode* childA = child->createChild({ 9.0f, 9.0f, -4.5f }, scaleLength);
			{
				BranchNode* childA1 = childA->createChild({ 7.0f, 4.0f, 2.0f }, scaleLength);
				{
					BranchNode* childA1a = childA1->createChild({ 2.0f, 2.0f, -1.0f }, scaleLength, true);
					module->tips.push_back(childA1a);

					BranchNode* childA1b = childA1->createChild({ 4.0f, 1.0f, 0.0f }, scaleLength);
					{
						BranchNode* childA1b1 = childA1b->createChild({ 2.0f, -0.5f, 1.0f }, scaleLength, true);
						module->tips.push_back(childA1b1);
						BranchNode* childA1b2 = childA1b->createChild({ 2.0f, 3.0f, -1.0f }, scaleLength, true);
						module->tips.push_back(childA1b2);
					}
				}

				BranchNode* childA2 = childA->createChild({ 2.0f, 7.0f, -3.0f }, scaleLength);
				{
					BranchNode* childA2a = childA2->createChild({ 0.5f, 4.0f, -2.0f }, scaleLength, true);
					module->tips.push_back(childA2a);
				}

			}

			BranchNode* childB = child->createChild({ 0.0f, 12.0f, 0.0f }, scaleLength);
			childB->main = true;
			{
				BranchNode* childB1 = childB->createChild({ 6.0f, 12.0f, 2.0f }, scaleLength);
				childB1->main = true;
				{
					BranchNode* childB1a = childB1->createChild({ 2.0f, 3.5f,0.0f }, scaleLength);
					childB1a->main = true;
					{
						BranchNode* childB1a1 = childB1a->createChild({ 1.0f, 6.0f, 0.0f }, scaleLength);
						childB1a1->main = true;
						{
							BranchNode* childB1a1a = childB1a1->createChild({ 1.5f, 3.0f,0.0f }, scaleLength, true);
							childB1a1a->main = true;
							module->tips.push_back(childB1a1a);
							BranchNode* childB1a1b = childB1a1->createChild({ -1.0f, 3.0f,0.0f }, scaleLength, true);
							module->tips.push_back(childB1a1b);
						}
					}

					BranchNode* childB1b = childB1->createChild({ -0.50f, 3.5f,0.0f }, scaleLength);
					{
						BranchNode* childB1b1 = childB1b->createChild({ 0.0f, 4.0f,0.0f }, scaleLength, true);
						module->tips.push_back(childB1b1);
					}
				}

				BranchNode* childB2 = childB->createChild({ -4.0f, 6.0f, -2.0f }, scaleLength);
				{
					BranchNode* childB2a = childB2->createChild({ -3.0f, 6.0f, -2.0f }, scaleLength, true);
					module->tips.push_back(childB2a);

					BranchNode* childB2b = childB2->createChild({ -1.5f, 1.0f,1.0f }, scaleLength);
					{
						BranchNode* childB21 = childB2b->createChild({ 0.5f, 2.0f,0.5f }, scaleLength, true);
						module->tips.push_back(childB21);
						BranchNode* childB22 = childB2b->createChild({ -2.0f, -1.0f, 1.0f }, scaleLength, true);
						module->tips.push_back(childB22);
					}
				}
			}

			BranchNode* childC = child->createChild({ -9.0f, 9.0f, -5.0f }, scaleLength);
			{
				BranchNode* childC1 = childC->createChild({ -3.0f, 12.0f,0.0f }, scaleLength);
				{
					BranchNode* childC1a = childC1->createChild({ 0.0f, 3.0f,0.0f }, scaleLength, true);
					BranchNode* childC1b = childC1->createChild({ -2.0f, 3.5f,0.0f }, scaleLength, true);
				}

				BranchNode* childC2 = childC->createChild({ -6.0f, 1.0f,0.0f }, scaleLength);
				{
					BranchNode* childC2a = childC2->createChild({ -5.0f, 2.0f,0.0f }, scaleLength, true);
					BranchNode* childC2b = childC2->createChild({ -2.0f, -1.0f,0.0f }, scaleLength, true);
				}
			}
		}
		return module;
	};
	modulePrototypeFunctions[0][2] = [=](BranchModule* module, bool removeFirst) -> BranchModule* {
		BranchNode* child = removeFirst ? module->root : module->root->createChild({ 0.0f, 12.0f, 0.0f }, scaleLength);
		child->main = true;
		{
			BranchNode* childA = child->createChild({ 5.0f, 5.0f, -2.0f }, scaleLength);
			{
				BranchNode* childA1 = childA->createChild({ 3.0f, 1.0f, 1.0f }, scaleLength, true);
				module->tips.push_back(childA1);
				BranchNode* childA2 = childA->createChild({ 4.0f, 5.0f, -2.0f }, scaleLength, true);
				module->tips.push_back(childA2);
				BranchNode* childA3 = childA->createChild({ 1.5f, 2.5f, -1.5f }, scaleLength, true);
				module->tips.push_back(childA3);
			}

			BranchNode* childB = child->createChild({ 0.0f, 7.0f, 0.0f }, scaleLength);
			childB->main = true;
			{
				BranchNode* childB1 = childB->createChild({ 5.0f, 9.5f, 0.5f }, scaleLength, true);
				module->tips.push_back(childB1);
				BranchNode* childB2 = childB->createChild({ 0.0f, 15.0f, 0.0f }, scaleLength);
				childB2->main = true;
				{
					BranchNode* childB2a = childB2->createChild({ 2.0f, 4.5f, 1.5f }, scaleLength, true);
					module->tips.push_back(childB2a);
					BranchNode* childB2b = childB2->createChild({ 0.0f, 6.5f, 0.0f }, scaleLength);
					childB2b->main = true;
					{
						BranchNode* childB2b1 = childB2b->createChild({ 3.5f, 6.0f, -0.5f }, scaleLength, true);
						module->tips.push_back(childB2b1);
						BranchNode* childB2b2 = childB2b->createChild({ 0.0f, 6.5f, 0.0f }, scaleLength, true);
						childB2b2->main = true;
						module->tips.push_back(childB2b2);
						BranchNode* childB2b3 = childB2b->createChild({ -3.0f, 6.0f, 0.5f }, scaleLength, true);
						module->tips.push_back(childB2b3);
					}
					BranchNode* childB2c = childB2->createChild({ -2.5f, 6.0f, -2.0f }, scaleLength, true);
					module->tips.push_back(childB2c);
				}

				BranchNode* childB3 = childB->createChild({ -3.0f, 4.5f, 1.5f }, scaleLength);
				{
					BranchNode* childB3a = childB3->createChild({ -1.5f, 3.0f, -0.5f }, scaleLength, true);
					module->tips.push_back(childB3a);
					BranchNode* childB3b = childB3->createChild({ -2.0f, 1.5f, 1.0f }, scaleLength, true);
					module->tips.push_back(childB3b);
					BranchNode* childB3c = childB3->createChild({ -1.0f, -0.5f, 0.2f }, scaleLength, true);
					module->tips.push_back(childB3c);
				}
			}

			BranchNode* childC = child->createChild({ -5.0f, 5.0f, -2.0f }, scaleLength);
			{
				BranchNode* childC1 = childC->createChild({ -0.2f, 3.5f, -1.0f }, scaleLength, true);
				module->tips.push_back(childC1);
				BranchNode* childC2 = childC->createChild({ -2.5f, 3.0f, -1.5f }, scaleLength);
				{
					BranchNode* childC2a = childC2->createChild({ -1.0f, 3.0f, -0.5f }, scaleLength, true);
					module->tips.push_back(childC2a);
					BranchNode* childC2b = childC2->createChild({ -2.0f, 2.0f, 0.0f }, scaleLength, true);
					module->tips.push_back(childC2b);
					BranchNode* childC2c = childC2->createChild({ -1.0f, 0.1f, 0.5f }, scaleLength, true);
					module->tips.push_back(childC2c);
				}
				BranchNode* childC3 = childC->createChild({ -3.0f, 0.5f, 1.5f }, scaleLength, true);
				module->tips.push_back(childC3);
			}
		}
		return module;
	};
	modulePrototypeFunctions[1][0] = [=](BranchModule* module, bool removeFirst) -> BranchModule* {
		BranchNode* child = removeFirst ? module->root : module->root->createChild({ 0.0f, 18.0f, 0.0f }, scaleLength);
		child->main = true;
		{
			BranchNode* childA = child->createChild({ 8.0f, 9.0f, 3.0f }, scaleLength);
			{
				BranchNode* childA1 = childA->createChild({ 7.5f, 3.5f, 2.0f }, scaleLength);
				{
					BranchNode* childA1a = childA1->createChild({ 5.5f, 3.0f, 1.0f }, scaleLength);
					{
						BranchNode* childA1a1 = childA1a->createChild({ 3.5f, -0.5f, 3.0f }, scaleLength, true);
						module->tips.push_back(childA1a1);
						BranchNode* childA1a2 = childA1a->createChild({ 3.0f, 3.0f, -1.5f }, scaleLength, true);
						module->tips.push_back(childA1a2);

					}
					BranchNode* childA1b = childA1->createChild({ 3.0f, 3.0f, -2.0f }, scaleLength, true);
					module->tips.push_back(childA1b);

				}
				BranchNode* childA2 = childA->createChild({ 1.5f, 7.5f, -5.0f }, scaleLength);
				{
					BranchNode* childA2a = childA2->createChild({ 1.5f, 5.0f, -3.0f }, scaleLength, true);
					module->tips.push_back(childA2a);
				}
			}

			BranchNode* childB = child->createChild({ -8.0f, 9.0f, 0.0f }, scaleLength);
			childB->main = true;
			{
				BranchNode* childB1 = childB->createChild({ -2.0f, 10.0f, -3.0f }, scaleLength);
				{
					childB1->main = true;
					BranchNode* childB1a = childB1->createChild({ 0.0f, 4.0f, -3.0f }, scaleLength);
					{
						BranchNode* childB1a1 = childB1a->createChild({ 3.0f, 1.5f, -1.5f }, scaleLength, true);
						module->tips.push_back(childB1a1);
						BranchNode* childB1a2 = childB1a->createChild({ -2.0f, 4.0f, 2.0f }, scaleLength, true);
						module->tips.push_back(childB1a2);

					}
					BranchNode* childB1b = childB1->createChild({ -2.5f, 5.0f, 2.0f }, scaleLength);
					{
						childB1b->main = true;
						BranchNode* childB1b1 = childB1b->createChild({ 0.5f, 2.0f, 0.0f }, scaleLength, true);
						module->tips.push_back(childB1b1);
						BranchNode* childB1b2 = childB1b->createChild({ -2.0f, 3.5f, 1.0f }, scaleLength);
						childB1b2->main = true;
						{
							BranchNode* childB1b2A = childB1b2->createChild({ 0.0f, 2.5f, -1.0f }, scaleLength, true);
							childB1b2A->main = true;
							module->tips.push_back(childB1b2A);
							BranchNode* childB1b2B = childB1b2->createChild({ -2.0f, 1.0f, 1.0f }, scaleLength, true);
							module->tips.push_back(childB1b2B);
						}
					}
				}
				BranchNode* childB2 = childB->createChild({ -8.0f, 2.0f, 4.0f }, scaleLength);
				{
					BranchNode* childB2a = childB2->createChild({ -5.5f, 3.0f, -2.0f }, scaleLength, true);
					module->tips.push_back(childB2a);
					BranchNode* childB2b = childB2->createChild({ -3.5f, -1.0f, 2.5f }, scaleLength);
					{
						BranchNode* childB2b1 = childB2b->createChild({ -1.0f, 1.0f, -1.0f }, scaleLength, true);
						module->tips.push_back(childB2b1);
						BranchNode* childB2b2 = childB2b->createChild({ -4.0f, -1.5f, 2.0f }, scaleLength, true);
						module->tips.push_back(childB2b2);
					}
				}
			}
		}
		return module;
	};
	modulePrototypeFunctions[1][1] = [=](BranchModule* module, bool removeFirst) -> BranchModule* {
		BranchNode* child = removeFirst ? module->root : module->root->createChild({ 0.0f, 18.0f, 0.0f }, scaleLength);
		child->main = true;
		{
			BranchNode* childA = child->createChild({ 9.0f, 9.0f, -4.5f }, scaleLength);
			{
				BranchNode* childA1 = childA->createChild({ 7.0f, 4.0f, 2.0f }, scaleLength);
				{
					BranchNode* childA1a = childA1->createChild({ 2.0f, 2.0f, -1.0f }, scaleLength, true);
					module->tips.push_back(childA1a);

					BranchNode* childA1b = childA1->createChild({ 4.0f, 1.0f, 0.0f }, scaleLength);
					{
						BranchNode* childA1b1 = childA1b->createChild({ 2.0f, -0.5f, 1.0f }, scaleLength, true);
						module->tips.push_back(childA1b1);
						BranchNode* childA1b2 = childA1b->createChild({ 2.0f, 3.0f, -1.0f }, scaleLength, true);
						module->tips.push_back(childA1b2);
					}
				}

				BranchNode* childA2 = childA->createChild({ 2.0f, 7.0f, -3.0f }, scaleLength);
				{
					BranchNode* childA2a = childA2->createChild({ 0.5f, 4.0f, -2.0f }, scaleLength, true);
					module->tips.push_back(childA2a);
				}

			}

			BranchNode* childB = child->createChild({ 0.0f, 12.0f, 0.0f }, scaleLength);
			childB->main = true;
			{
				BranchNode* childB1 = childB->createChild({ 6.0f, 12.0f, 2.0f }, scaleLength);
				childB1->main = true;
				{
					BranchNode* childB1a = childB1->createChild({ 2.0f, 3.5f,0.0f }, scaleLength);
					childB1a->main = true;
					{
						BranchNode* childB1a1 = childB1a->createChild({ 1.0f, 6.0f, 0.0f }, scaleLength);
						childB1a1->main = true;
						{
							BranchNode* childB1a1a = childB1a1->createChild({ 1.5f, 3.0f,0.0f }, scaleLength, true);
							childB1a1a->main = true;
							module->tips.push_back(childB1a1a);
							BranchNode* childB1a1b = childB1a1->createChild({ -1.0f, 3.0f,0.0f }, scaleLength, true);
							module->tips.push_back(childB1a1b);
						}
					}

					BranchNode* childB1b = childB1->createChild({ -0.50f, 3.5f,0.0f }, scaleLength);
					{
						BranchNode* childB1b1 = childB1b->createChild({ 0.0f, 4.0f,0.0f }, scaleLength, true);
						module->tips.push_back(childB1b1);
					}
				}

				BranchNode* childB2 = childB->createChild({ -4.0f, 6.0f, -2.0f }, scaleLength);
				{
					BranchNode* childB2a = childB2->createChild({ -3.0f, 6.0f, -2.0f }, scaleLength, true);
					module->tips.push_back(childB2a);

					BranchNode* childB2b = childB2->createChild({ -1.5f, 1.0f,1.0f }, scaleLength);
					{
						BranchNode* childB21 = childB2b->createChild({ 0.5f, 2.0f,0.5f }, scaleLength, true);
						module->tips.push_back(childB21);
						BranchNode* childB22 = childB2b->createChild({ -2.0f, -1.0f, 1.0f }, scaleLength, true);
						module->tips.push_back(childB22);
					}
				}
			}

			BranchNode* childC = child->createChild({ -9.0f, 9.0f, -5.0f }, scaleLength);
			{
				BranchNode* childC1 = childC->createChild({ -3.0f, 12.0f,0.0f }, scaleLength);
				{
					BranchNode* childC1a = childC1->createChild({ 0.0f, 3.0f,0.0f }, scaleLength, true);
					BranchNode* childC1b = childC1->createChild({ -2.0f, 3.5f,0.0f }, scaleLength, true);
				}

				BranchNode* childC2 = childC->createChild({ -6.0f, 1.0f,0.0f }, scaleLength);
				{
					BranchNode* childC2a = childC2->createChild({ -5.0f, 2.0f,0.0f }, scaleLength, true);
					BranchNode* childC2b = childC2->createChild({ -2.0f, -1.0f,0.0f }, scaleLength, true);
				}
			}
		}
		return module;
	};
	modulePrototypeFunctions[1][2] = [=](BranchModule* module, bool removeFirst) -> BranchModule* {
		BranchNode* child = removeFirst ? module->root : module->root->createChild({ 0.0f, 12.0f, 0.0f }, scaleLength);
		child->main = true;
		{
			BranchNode* childA = child->createChild({ 5.0f, 5.0f, -2.0f }, scaleLength);
			{
				BranchNode* childA1 = childA->createChild({ 3.0f, 1.0f, 1.0f }, scaleLength, true);
				module->tips.push_back(childA1);
				BranchNode* childA2 = childA->createChild({ 4.0f, 5.0f, -2.0f }, scaleLength, true);
				module->tips.push_back(childA2);
				BranchNode* childA3 = childA->createChild({ 1.5f, 2.5f, -1.5f }, scaleLength, true);
				module->tips.push_back(childA3);
			}

			BranchNode* childB = child->createChild({ 0.0f, 7.0f, 0.0f }, scaleLength);
			childB->main = true;
			{
				BranchNode* childB1 = childB->createChild({ 5.0f, 9.5f, 0.5f }, scaleLength, true);
				module->tips.push_back(childB1);
				BranchNode* childB2 = childB->createChild({ 0.0f, 15.0f, 0.0f }, scaleLength);
				childB2->main = true;
				{
					BranchNode* childB2a = childB2->createChild({ 2.0f, 4.5f, 1.5f }, scaleLength, true);
					module->tips.push_back(childB2a);
					BranchNode* childB2b = childB2->createChild({ 0.0f, 6.5f, 0.0f }, scaleLength);
					childB2b->main = true;
					{
						BranchNode* childB2b1 = childB2b->createChild({ 3.5f, 6.0f, -0.5f }, scaleLength, true);
						module->tips.push_back(childB2b1);
						BranchNode* childB2b2 = childB2b->createChild({ 0.0f, 6.5f, 0.0f }, scaleLength, true);
						childB2b2->main = true;
						module->tips.push_back(childB2b2);
						BranchNode* childB2b3 = childB2b->createChild({ -3.0f, 6.0f, 0.5f }, scaleLength, true);
						module->tips.push_back(childB2b3);
					}
					BranchNode* childB2c = childB2->createChild({ -2.5f, 6.0f, -2.0f }, scaleLength, true);
					module->tips.push_back(childB2c);
				}

				BranchNode* childB3 = childB->createChild({ -3.0f, 4.5f, 1.5f }, scaleLength);
				{
					BranchNode* childB3a = childB3->createChild({ -1.5f, 3.0f, -0.5f }, scaleLength, true);
					module->tips.push_back(childB3a);
					BranchNode* childB3b = childB3->createChild({ -2.0f, 1.5f, 1.0f }, scaleLength, true);
					module->tips.push_back(childB3b);
					BranchNode* childB3c = childB3->createChild({ -1.0f, -0.5f, 0.2f }, scaleLength, true);
					module->tips.push_back(childB3c);
				}
			}

			BranchNode* childC = child->createChild({ -5.0f, 5.0f, -2.0f }, scaleLength);
			{
				BranchNode* childC1 = childC->createChild({ -0.2f, 3.5f, -1.0f }, scaleLength, true);
				module->tips.push_back(childC1);
				BranchNode* childC2 = childC->createChild({ -2.5f, 3.0f, -1.5f }, scaleLength);
				{
					BranchNode* childC2a = childC2->createChild({ -1.0f, 3.0f, -0.5f }, scaleLength, true);
					module->tips.push_back(childC2a);
					BranchNode* childC2b = childC2->createChild({ -2.0f, 2.0f, 0.0f }, scaleLength, true);
					module->tips.push_back(childC2b);
					BranchNode* childC2c = childC2->createChild({ -1.0f, 0.1f, 0.5f }, scaleLength, true);
					module->tips.push_back(childC2c);
				}
				BranchNode* childC3 = childC->createChild({ -3.0f, 0.5f, 1.5f }, scaleLength, true);
				module->tips.push_back(childC3);
			}
		}
		return module;
	};
	modulePrototypeFunctions[2][0] = [=](BranchModule* module, bool removeFirst) -> BranchModule* {
		BranchNode* child = removeFirst ? module->root : module->root->createChild({ 0.0f, 18.0f, 0.0f }, scaleLength);
		child->main = true;
		{
			BranchNode* childA = child->createChild({ 3.0f, 8.0f, -3.0f }, scaleLength);
			{
				BranchNode* childA1 = childA->createChild({ 6.0f, 8.0f, -1.0f }, scaleLength);
				{
					BranchNode* childA1a = childA1->createChild({ 2.0f, 0.5f, 2.0f }, scaleLength, true);
					module->tips.push_back(childA1a);
					BranchNode* childA1b = childA1->createChild({ 1.0f, 4.0f, -0.5f }, scaleLength, true);
					module->tips.push_back(childA1b);
				}
				BranchNode* childA2 = childA->createChild({ -0.5f, 4.0f, -1.0f }, scaleLength, true);
				module->tips.push_back(childA2);
			}

			BranchNode* childB = child->createChild({ -4.0f, 11.0f, 2.0f }, scaleLength);
			childB->main = true;
			{
				BranchNode* childB1 = childB->createChild({ 0.0f, 9.0f, 1.5f }, scaleLength);
				childB1->main = true;
				{
					BranchNode* childB1a = childB1->createChild({ -0.5f, 2.5f, 0.0f }, scaleLength);
					childB1a->main = true;
					{
						BranchNode* childB1a1 = childB1a->createChild({ 3.0f, 2.0f, -2.0f }, scaleLength, true);
						module->tips.push_back(childB1a1);
						BranchNode* childB1a2 = childB1a->createChild({ 0.2f, 2.5f, -2.5f }, scaleLength, true);
						module->tips.push_back(childB1a2);
						BranchNode* childB1a3 = childB1a->createChild({ -1.5f, 5.0f, 2.0f }, scaleLength);
						childB1a3->main = true;
						{
							BranchNode* childB1a3A = childB1a3->createChild({ 1.0f, 3.0f, 1.0f }, scaleLength);
							childB1a3A->main = true;
							{
								BranchNode* childB1a3A1 = childB1a3A->createChild({ 1.0f, 1.0f, 1.0f }, scaleLength, true);
								module->tips.push_back(childB1a3A1);
								BranchNode* childB1a3A2 = childB1a3A->createChild({ -1.0f, 1.0f, -0.5f }, scaleLength);
								childB1a3A2->main = true;
								{
									BranchNode* childB1a3A1a = childB1a3A2->createChild({ 0.0f, 2.0f, -1.0f }, scaleLength, true);
									childB1a3A1a->main = true;
									module->tips.push_back(childB1a3A1a);
								}
							}
							BranchNode* childB1a3B = childB1a3->createChild({ -2.0f, 1.0f, 1.0f }, scaleLength, true);
							module->tips.push_back(childB1a3B);
						}
					}
				}
				BranchNode* childB2 = childB->createChild({ -3.5f, 4.0f, -3.0f }, scaleLength);
				{
						BranchNode* childB2a = childB2->createChild({ -2.0f, 3.0f, -2.0f }, scaleLength, true);
						module->tips.push_back(childB2a);
						BranchNode* childB2b = childB2->createChild({ -2.0f, 1.0f, 1.0f }, scaleLength);
						{
							BranchNode* childB2b1 = childB2b->createChild({ 0.0f, 2.0f, -1.0f }, scaleLength, true);
							module->tips.push_back(childB2b1);
							BranchNode* childB2b2 = childB2b->createChild({ -2.0f, -0.5f, 1.0f }, scaleLength, true);
							module->tips.push_back(childB2b2);
						}
				}
			}
		}
		return module;
	};
	modulePrototypeFunctions[2][1] = [=](BranchModule* module, bool removeFirst) -> BranchModule* {
		BranchNode* child = removeFirst ? module->root : module->root->createChild({ 0.0f, 18.0f, 0.0f }, scaleLength);
		child->main = true;
		{
			BranchNode* childA = child->createChild({ 9.0f, 9.0f, -4.5f }, scaleLength);
			{
				BranchNode* childA1 = childA->createChild({ 7.0f, 4.0f, 2.0f }, scaleLength);
				{
					BranchNode* childA1a = childA1->createChild({ 2.0f, 2.0f, -1.0f }, scaleLength, true);
					module->tips.push_back(childA1a);

					BranchNode* childA1b = childA1->createChild({ 4.0f, 1.0f, 0.0f }, scaleLength);
					{
						BranchNode* childA1b1 = childA1b->createChild({ 2.0f, -0.5f, 1.0f }, scaleLength, true);
						module->tips.push_back(childA1b1);
						BranchNode* childA1b2 = childA1b->createChild({ 2.0f, 3.0f, -1.0f }, scaleLength, true);
						module->tips.push_back(childA1b2);
					}
				}

				BranchNode* childA2 = childA->createChild({ 2.0f, 7.0f, -3.0f }, scaleLength);
				{
					BranchNode* childA2a = childA2->createChild({ 0.5f, 4.0f, -2.0f }, scaleLength, true);
					module->tips.push_back(childA2a);
				}

			}

			BranchNode* childB = child->createChild({ 0.0f, 12.0f, 0.0f }, scaleLength);
			childB->main = true;
			{
				BranchNode* childB1 = childB->createChild({ 6.0f, 12.0f, 2.0f }, scaleLength);
				childB1->main = true;
				{
					BranchNode* childB1a = childB1->createChild({ 2.0f, 3.5f,0.0f }, scaleLength);
					childB1a->main = true;
					{
						BranchNode* childB1a1 = childB1a->createChild({ 1.0f, 6.0f, 0.0f }, scaleLength);
						childB1a1->main = true;
						{
							BranchNode* childB1a1a = childB1a1->createChild({ 1.5f, 3.0f,0.0f }, scaleLength, true);
							childB1a1a->main = true;
							module->tips.push_back(childB1a1a);
							BranchNode* childB1a1b = childB1a1->createChild({ -1.0f, 3.0f,0.0f }, scaleLength, true);
							module->tips.push_back(childB1a1b);
						}
					}

					BranchNode* childB1b = childB1->createChild({ -0.50f, 3.5f,0.0f }, scaleLength);
					{
						BranchNode* childB1b1 = childB1b->createChild({ 0.0f, 4.0f,0.0f }, scaleLength, true);
						module->tips.push_back(childB1b1);
					}
				}

				BranchNode* childB2 = childB->createChild({ -4.0f, 6.0f, -2.0f }, scaleLength);
				{
					BranchNode* childB2a = childB2->createChild({ -3.0f, 6.0f, -2.0f }, scaleLength, true);
					module->tips.push_back(childB2a);

					BranchNode* childB2b = childB2->createChild({ -1.5f, 1.0f,1.0f }, scaleLength);
					{
						BranchNode* childB21 = childB2b->createChild({ 0.5f, 2.0f,0.5f }, scaleLength, true);
						module->tips.push_back(childB21);
						BranchNode* childB22 = childB2b->createChild({ -2.0f, -1.0f, 1.0f }, scaleLength, true);
						module->tips.push_back(childB22);
					}
				}
			}

			BranchNode* childC = child->createChild({ -9.0f, 9.0f, -5.0f }, scaleLength);
			{
				BranchNode* childC1 = childC->createChild({ -3.0f, 12.0f,0.0f }, scaleLength);
				{
					BranchNode* childC1a = childC1->createChild({ 0.0f, 3.0f,0.0f }, scaleLength, true);
					BranchNode* childC1b = childC1->createChild({ -2.0f, 3.5f,0.0f }, scaleLength, true);
				}

				BranchNode* childC2 = childC->createChild({ -6.0f, 1.0f,0.0f }, scaleLength);
				{
					BranchNode* childC2a = childC2->createChild({ -5.0f, 2.0f,0.0f }, scaleLength, true);
					BranchNode* childC2b = childC2->createChild({ -2.0f, -1.0f,0.0f }, scaleLength, true);
				}
			}
		}
		return module;
	};
	modulePrototypeFunctions[2][2] = [=](BranchModule* module, bool removeFirst) -> BranchModule* {
		BranchNode* child = removeFirst ? module->root : module->root->createChild({ 0.0f, 12.0f, 0.0f }, scaleLength);
		child->main = true;
		{
			BranchNode* childA = child->createChild({ 5.0f, 5.0f, -2.0f }, scaleLength);
			{
				BranchNode* childA1 = childA->createChild({ 3.0f, 1.0f, 1.0f }, scaleLength, true);
				module->tips.push_back(childA1);
				BranchNode* childA2 = childA->createChild({ 4.0f, 5.0f, -2.0f }, scaleLength, true);
				module->tips.push_back(childA2);
				BranchNode* childA3 = childA->createChild({ 1.5f, 2.5f, -1.5f }, scaleLength, true);
				module->tips.push_back(childA3);
			}

			BranchNode* childB = child->createChild({ 0.0f, 7.0f, 0.0f }, scaleLength);
			childB->main = true;
			{
				BranchNode* childB1 = childB->createChild({ 5.0f, 9.5f, 0.5f }, scaleLength, true);
				module->tips.push_back(childB1);
				BranchNode* childB2 = childB->createChild({ 0.0f, 15.0f, 0.0f }, scaleLength);
				childB2->main = true;
				{
					BranchNode* childB2a = childB2->createChild({ 2.0f, 4.5f, 1.5f }, scaleLength, true);
					module->tips.push_back(childB2a);
					BranchNode* childB2b = childB2->createChild({ 0.0f, 6.5f, 0.0f }, scaleLength);
					childB2b->main = true;
					{
						BranchNode* childB2b1 = childB2b->createChild({ 3.5f, 6.0f, -0.5f }, scaleLength, true);
						module->tips.push_back(childB2b1);
						BranchNode* childB2b2 = childB2b->createChild({ 0.0f, 6.5f, 0.0f }, scaleLength, true);
						childB2b2->main = true;
						module->tips.push_back(childB2b2);
						BranchNode* childB2b3 = childB2b->createChild({ -3.0f, 6.0f, 0.5f }, scaleLength, true);
						module->tips.push_back(childB2b3);
					}
					BranchNode* childB2c = childB2->createChild({ -2.5f, 6.0f, -2.0f }, scaleLength, true);
					module->tips.push_back(childB2c);
				}

				BranchNode* childB3 = childB->createChild({ -3.0f, 4.5f, 1.5f }, scaleLength);
				{
					BranchNode* childB3a = childB3->createChild({ -1.5f, 3.0f, -0.5f }, scaleLength, true);
					module->tips.push_back(childB3a);
					BranchNode* childB3b = childB3->createChild({ -2.0f, 1.5f, 1.0f }, scaleLength, true);
					module->tips.push_back(childB3b);
					BranchNode* childB3c = childB3->createChild({ -1.0f, -0.5f, 0.2f }, scaleLength, true);
					module->tips.push_back(childB3c);
				}
			}

			BranchNode* childC = child->createChild({ -5.0f, 5.0f, -2.0f }, scaleLength);
			{
				BranchNode* childC1 = childC->createChild({ -0.2f, 3.5f, -1.0f }, scaleLength, true);
				module->tips.push_back(childC1);
				BranchNode* childC2 = childC->createChild({ -2.5f, 3.0f, -1.5f }, scaleLength);
				{
					BranchNode* childC2a = childC2->createChild({ -1.0f, 3.0f, -0.5f }, scaleLength, true);
					module->tips.push_back(childC2a);
					BranchNode* childC2b = childC2->createChild({ -2.0f, 2.0f, 0.0f }, scaleLength, true);
					module->tips.push_back(childC2b);
					BranchNode* childC2c = childC2->createChild({ -1.0f, 0.1f, 0.5f }, scaleLength, true);
					module->tips.push_back(childC2c);
				}
				BranchNode* childC3 = childC->createChild({ -3.0f, 0.5f, 1.5f }, scaleLength, true);
				module->tips.push_back(childC3);
			}
		}
		return module;
	};
}

BranchModule* Morphospace::selectModule(float apicalControl, float determinacy, BranchNode*& root, bool removeFirst) {
	/*
	Apical control and determinacy are always between 0 and 1 so to 
	obtain the module in the morphospace we multiply apical control and determinacy by 3 and round the result up and subtract 1 which 
	gives an index that is able to access the array with the modules creation functions
	with this approach we remove all ifs from the function. 

	Example: apical control 0.3, determinacy = 0.5
	idxApicalControl = 0;
	idxDeterminacy = 1;

	The final array position will be [0][1]

	This is a very simplified implementation of voronoi partioning where the regions are squares with the exact same area
	In our case each square has an area of 0.333..*0.333..
	*/

	int idxApicalControl = apicalControl == 1 ? 2 : int(apicalControl * 3.0f);
	int idxDeterminacy = determinacy == 1 ? 2 : int(determinacy * 3.0f);

	BranchModule* module = new BranchModule(root);

	return modulePrototypeFunctions[idxApicalControl][idxDeterminacy](module, removeFirst);
}