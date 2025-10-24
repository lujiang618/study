package main

import (
	"image"
	"image/draw"
	_ "image/jpeg"
	_ "image/png"
	"os"
)

func main() {
	file, err := os.Open("./4.jpg")
	if err != nil {
		panic(err)
	}

	defer file.Close()

	img, _, err := image.Decode(file)
	if err != nil {
		panic(err)
	}

	rgbaImage := image.NewRGBA(img.Bounds())
	draw.Draw(rgbaImage, img.Bounds(), img, img.Bounds().Min, draw.Src)

	outFile, err := os.Create("pixels.txt")
	if err != nil {
		panic(err)
	}
	defer outFile.Close()

	outFile.Write(rgbaImage.Pix)
}
