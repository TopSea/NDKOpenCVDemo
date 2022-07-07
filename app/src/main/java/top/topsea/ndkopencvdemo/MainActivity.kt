package top.topsea.ndkopencvdemo

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.SeekBar
import top.topsea.ndkopencvdemo.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private var image: Bitmap? = null

    private var flippedHor = false
    private var flippedVer = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        image = BitmapFactory.decodeResource(resources, R.drawable.e12)

        binding.imageView.setImageBitmap(image)
        binding.txtCvVersion.text = getString(R.string.cv_version, getOpenCVVersion())

        allClicks()
    }

    private external fun getOpenCVVersion(): String
    private external fun blurBitmap(bitmap: Bitmap = image!!, width: Int, height: Int): Bitmap
    private external fun flipBitmap(bitmap: Bitmap = image!!, flipCode: Int): Bitmap
    private external fun waterMark(bitmap: Bitmap = image!!, mark: Bitmap, witchCorner: Int): Bitmap
    private external fun saltBitmap(bitmap: Bitmap = image!!, times: Int): Bitmap
    private external fun colorReduce(bitmap: Bitmap = image!!, div : Int): Bitmap
    private external fun mergeBitmap(bitmap: Bitmap = image!!, mark: Bitmap, mergeWeight: Float): Bitmap
    private external fun maskBitmap(bitmap: Bitmap = image!!): Bitmap

    private fun allClicks() {
        binding.btnBlur.setOnClickListener {
            var imgWidth = image!!.width
            var imgHeight = image!!.height
            Log.d("TopSea:::", "imgWidth: $imgWidth")
            Log.d("TopSea:::", "imgHeight: $imgHeight")

            imgWidth /= 100
            imgHeight /= 100
            Log.d("TopSea:::", "imgWidth: $imgWidth")
            Log.d("TopSea:::", "imgHeight: $imgHeight")

            image = blurBitmap(width = imgWidth, height = imgHeight)
            binding.imageView.setImageBitmap(image)
        }

        //长按还原模糊效果
        binding.btnBlur.setOnLongClickListener {
            image = BitmapFactory.decodeResource(resources, R.drawable.e12)
            if (flippedHor) {
                image = flipBitmap(flipCode = 1)
            }
            if (flippedVer) {
                image = flipBitmap(flipCode = 0)
            }
            binding.imageView.setImageBitmap(image)
            true
        }

        binding.btnFlip.setOnClickListener {
            binding.flipOri.visibility = if (binding.flipOri.visibility == View.GONE) {
                View.VISIBLE
            } else {
                View.GONE
            }
        }

        binding.flipHor.setOnClickListener {
            image = flipBitmap(flipCode = 1)
            flippedHor = !flippedHor
            binding.imageView.setImageBitmap(image)
        }

        binding.flipVer.setOnClickListener {
            image = flipBitmap(flipCode = 0)
            flippedVer = !flippedVer
            binding.imageView.setImageBitmap(image)
        }

        binding.btnMark.setOnClickListener {
            val mark = BitmapFactory.decodeResource(resources, R.drawable.topsea)
            image = waterMark(bitmap = image!!, mark = mark, witchCorner = 1)
            binding.imageView.setImageBitmap(image)
        }

        binding.btnSalt.setOnClickListener {
            image = saltBitmap(times = 50000)
            binding.imageView.setImageBitmap(image)
        }

        binding.btnMerge.setOnClickListener {
            val mark = BitmapFactory.decodeResource(resources, R.drawable.cherry)
            image = mergeBitmap(bitmap = image!!, mark = mark, mergeWeight = 0.5f)
            binding.imageView.setImageBitmap(image)
        }

        binding.btnCrReduce.setOnClickListener {
            image = colorReduce(div = 64)
            binding.imageView.setImageBitmap(image)
        }

        binding.btnMask.setOnClickListener {
            image = maskBitmap()
            binding.imageView.setImageBitmap(image)
        }
    }

    companion object {
        // Used to load the 'opencv' library on application startup.
        init {
            System.loadLibrary("ndkopencvdemo")
        }
    }
}