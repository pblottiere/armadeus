/* 
 * pcie_msi.c - Driver to register an MSI interrupt in kernel
 *
 * Copyright 2015 Fabien Marteau fabien.marteau@armadeus.com
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/device.h>

#define DRV_NAME	"pcie_msi"
#define DRV_VERSION	"0.1"

static irqreturn_t interrupt_msi(int irq, void *data)
{
	printk("msi %d\n", irq);
	return IRQ_HANDLED;
}

static int msi_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	int ret = -EAGAIN;
	int msi_num = 0;
	
	/* acquire resources */
	ret = pcim_enable_device(pdev);
	if (ret)
		return ret;
	/* Set up a single MSI interrupt */
	msi_num = pci_enable_msi_range(pdev, 1, 4);
	if (msi_num) {
		ret = request_irq(pdev->irq, interrupt_msi, 0, "test_msi_pcie", NULL);
		if(ret) {
			printk("can't request msi interrupt\n");
			pci_disable_msi(pdev);
		}

	}
	printk("%d: %d msi registered\n", pdev->irq, msi_num);

	pci_set_master(pdev);

	return 0;
}

static void msi_remove(struct pci_dev *pdev)
{
	pci_clear_master(pdev);
	free_irq(pdev->irq, NULL);
	pci_disable_msi(pdev);
	pci_disable_device(pdev);
}

static const struct pci_device_id msi_pci_tbl[] = {
	{ PCI_DEVICE(0x1172, 0xE001), 0},
	{}
};

static struct pci_driver msi_pci_driver = {
	.name			= DRV_NAME,
	.id_table		= msi_pci_tbl,
	.probe			= msi_probe,
	.remove			= msi_remove,
};

module_pci_driver(msi_pci_driver);
MODULE_AUTHOR("Fabien Marteau");
MODULE_DESCRIPTION("Register an MSI interrupt");
MODULE_LICENSE("GPL");
MODULE_DEVICE_TABLE(pci, msi_pci_tbl);
